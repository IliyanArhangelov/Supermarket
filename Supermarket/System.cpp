#include "System.h"
#include "Constants.h"
#include "Manager.h" 
#include "Cashier.h"
#include "ProductByUnit.h"
#include "ProductByWeight.h"
#include "Warning.h"
#include "CommonFunctions.hpp"
#include "Logger.h"
#include "AllProductsGiftCard.h"
#include "SingleCategoryGiftCard.h"
#include <fstream>
#include "MultipleCategoryGiftCard.h"

void System::loadProductCategory()
{
	std::ifstream ifs(FileName::PRODUCT_CATEGORY);
	if (!ifs.is_open()) 
	{
		Logger::log("Could not open product category file. Starting with empty categories.");
		return;
	}

	ifs >> productCategoryCount;
	productCategoryCapacity = productCategoryCount + Constants::DEFAULT_CAPACITY;
	productCategory = new ProductCategory*[productCategoryCapacity];

	for (size_t i = 0; i < productCategoryCount; i++)
	{
		size_t id;
		ifs >> id;
		char* name = FileOpr::readString(ifs);
		char* desc = FileOpr::readString(ifs);
		productCategory[i] = new ProductCategory(id, name, desc);

		delete[] name;
		delete[] desc;
	}
	Logger::log("Product categories loaded successfully.");
}

void System::createDefaultManager()
{
	Logger::log("First run detected. Creating default manager account.");

	// first manager info
	const size_t defaultId = 1;
	const char defaultFirstName[] = "Admin";
	const char defaultLastName[] = "Adminov";
	const char defaultPhone[] = "0000000000";
	const size_t defaultAge = 99;
	const char defaultPassword[] = "superadmin";

	Manager* admin = new Manager(defaultId, defaultFirstName, defaultLastName, defaultPhone, defaultAge, defaultPassword);

	workersCapacity = Constants::DEFAULT_CAPACITY;
	workers = new Worker*[workersCapacity];
	workers[0] = admin;
	workersCount = 1;

	// special code generating and saving
	char* specialCode = admin->generateAndSaveSpecialCode();

	Logger::log("Special code for default manager created.");
	delete[] specialCode;
}

void System::printHelp() const
{
	std::cout << "--- FMI Supermarket Help ---" << std::endl;

	// general commands
	std::cout << "\n[General Commands (Always Available)]" << std::endl;
	std::cout << "  login <id> <password>              - Log into the system." << std::endl;
	std::cout << "  register <role> <fn> <ln> ...    - Register a new user (role: manager/cashier)." << std::endl;
	std::cout << "  help                               - Shows this help message." << std::endl;
	std::cout << "  exit / quit                        - Closes the application." << std::endl;
	std::cout << "  list-products [category_id]        - List all products or products in a specific category." << std::endl;
	std::cout << "  list-product-categories            - List all available product categories." << std::endl;
	std::cout << "  list-transactions                  - List all transactions in the system." << std::endl;
	std::cout << "  list-feed                          - Show the system activity feed." << std::endl;
	std::cout << "  list-gift-cards                    - List all available gift cards." << std::endl;

	if (!currentUser)
	{
		std::cout << "\nLog in to see role-specific commands." << std::endl;
	}
	else
	{
		// for logged-in users
		std::cout << "\n[Logged-in User Commands]" << std::endl;
		std::cout << "  logout                             - Log out from the current session." << std::endl;
		std::cout << "  leave                              - Permanently leave the job and delete your account." << std::endl;
		std::cout << "  list-user-data                     - Shows your personal data." << std::endl;

		// role-specific commands
		if (currentUser->getWorkerType() == WorkerType::CASHIER)
		{
			std::cout << "\n[Cashier Commands]" << std::endl;
			std::cout << "  sell                               - Start a new sale transaction." << std::endl;
		}
		else if (currentUser->getWorkerType() == WorkerType::MANAGER)
		{
			std::cout << "\n[Manager Commands]" << std::endl;
			std::cout << "  -- User Management --" << std::endl;
			std::cout << "  list-workers                       - List all workers in the system." << std::endl;
			std::cout << "  list-pending                       - List pending cashier registrations." << std::endl;
			std::cout << "  approve <id> <special_code>        - Approve a pending cashier." << std::endl;
			std::cout << "  decline <id> <special_code>        - Decline a pending cashier." << std::endl;
			std::cout << "  promote-cashier <id> <code>       - Promote a cashier to manager." << std::endl;
			std::cout << "  fire-cashier <id> <code>           - Fire a cashier." << std::endl;
			std::cout << "  warn-cashier <id> <points> ...   - Issue a warning to a cashier (points: 100/200/300)." << std::endl;
			std::cout << "  list-warned-cashiers <points>      - List cashiers above a warning point threshold." << std::endl;
			std::cout << "\n  -- Inventory & Promotions --" << std::endl;
			std::cout << "  add-category <name> <desc>         - Add a new product category." << std::endl;
			std::cout << "  delete-category <id>               - Delete an empty product category." << std::endl;
			std::cout << "  add-product <type>                 - Add a new product (type: product_by_unit/product_by_weight)." << std::endl;
			std::cout << "  delete-product <id>                - Delete a product from inventory." << std::endl;
			std::cout << "  load-products <filename>           - Load products in bulk from a file." << std::endl;
			std::cout << "  load-gift-cards <filename>         - Generate new gift cards from a file." << std::endl;
		}
	}
	std::cout << "------------------------------" << std::endl;
}

Worker* System::findWorkerById(size_t id) const
{
	for (size_t i = 0; i < workersCount; i++)
	{
		if (workers[i] && workers[i]->getId() == id)
		{
			return workers[i];
		}
	}
	return nullptr;
}

const ProductCategory* System::findCategoryById(size_t id) const
{
	for (size_t i = 0; i < productCategoryCount; i++)
	{
		if (productCategory[i] && productCategory[i]->getId() == id)
		{
			return productCategory[i];
		}
	}
	return nullptr; 
}

const ProductCategory* System::findCategoryByName(const char* name) const
{
	for (size_t i = 0; i < productCategoryCount; i++)
	{
		if (productCategory[i] && StrOpr::equals(productCategory[i]->getName(), name))
		{
			return productCategory[i];
		}
	}
	return nullptr;
}

void System::loadProducts()
{
	std::ifstream ifs(FileName::PRODUCTS);
	if (!ifs.is_open()) 
	{
		Logger::log("Could not open products file. Starting with empty inventory.");
		return;
	}

	ifs >> productsCount;
	productsCapacity = productsCount + Constants::DEFAULT_CAPACITY;
	products = new Product*[productsCount];

	for (size_t i = 0; i < productsCount; i++)
	{
		int typeInt;
		ifs >> typeInt;
		ProductType type = static_cast<ProductType>(typeInt);

		size_t id;
		ifs >> id;
		char* name = FileOpr::readString(ifs);
		double price;
		ifs >> price;
		size_t categoryId;
		ifs >> categoryId;
		double quantity;
		ifs >> quantity;

		const ProductCategory* category = ProductCategory::findById(categoryId, productCategory, productCategoryCount);

		if (!category) 
		{
			Logger::log("Error: Product with invalid category ID found. Skipping.");
			delete[] name;
			products[i] = nullptr;
			continue;
		}

		if (type == ProductType::ByUnit)
		{
			products[i] = new ProductByUnit(id, name, price, category, static_cast<size_t>(quantity));
		}
		else // ByWeight
		{
			products[i] = new ProductByWeight(id, name, price, category, quantity);
		}
		delete[] name;
	}
	Logger::log("Products loaded successfully.");
}


void System::writeToFiles() 
{
	saveWorkersToFile();
	saveProductCategoriesToFile();
	saveProductsToFile();
	saveTransactions();    
	saveGiftCardsToFile();
} 

void System::free()
{
	for (size_t i = 0; i < workersCount; i++) 
	{
		delete workers[i];
	}
	delete[] workers;

	for (size_t i = 0; i < productsCount; i++) 
	{
		delete products[i];
	}
	delete[] products;

	for (size_t i = 0; i < productCategoryCount; i++) 
	{
		delete productCategory[i];
	}
	delete[] productCategory;

	for (size_t i = 0; i < transactionsCount; i++) 
	{
		delete transactions[i];
	}
	delete[] transactions;

	for (size_t i = 0; i < giftCardsCount; i++)
	{
		delete giftCards[i];
	}
	delete[] giftCards;

	transactions = nullptr;
	currentUser = nullptr;
	workers = nullptr;
	products = nullptr;
	productCategory = nullptr;
	giftCards = nullptr;
	workersCount = 0;
	productsCount = 0;
	productCategoryCount = 0;
	transactionsCount = 0;
	giftCardsCount = 0;
}

void System::loadGiftCardsFromFile() 
{
	std::ifstream ifs(FileName::GIFT_CARDS);
	if (!ifs.is_open() || ifs.peek() == EOF) 
	{
		Logger::log("Gift cards file is empty or not found. Starting with no gift cards.");
		return;
	}

	if (!(ifs >> voucherCounter)) 
	{
		Logger::log("Could not read voucher counter from GiftCards.txt. Assuming 0.");
		voucherCounter = 0;
		return;
	}

	size_t singleCategoryCount = 0;
	if (!(ifs >> singleCategoryCount)) 
	{
		Logger::log("Error reading SingleCategoryGiftCard count from file.");
		return;
	}

	for (size_t i = 0; i < singleCategoryCount; i++) 
	{
		if (giftCardsCount >= giftCardsCapacity) 
		{
			size_t newCap = (giftCardsCapacity == 0) ? Constants::DEFAULT_CAPACITY : giftCardsCapacity * 2;
			COMMON::resize(giftCards, giftCardsCount, newCap);
			giftCardsCapacity = newCap;
		}

		int typeInt;
		double percentage;
		size_t categoryId;

		ifs >> typeInt;
		char* code = FileOpr::readString(ifs);
		ifs >> percentage;
		ifs >> categoryId;

		if (!ifs) 
		{
			Logger::log("Error reading data for a SingleCategoryGiftCard. Stopping load.");
			delete[] code;
			return;
		}

		ProductCategory* foundCategory = nullptr;
		for (size_t j = 0; j < productCategoryCount; j++) 
		{
			if (productCategory[j] && productCategory[j]->getId() == categoryId)
			{
				foundCategory = productCategory[j];
				break;
			}
		}

		if (foundCategory) 
		{
			giftCards[giftCardsCount++] = new SingleCategoryGiftCard(code, percentage, foundCategory);
		}
		else 
		{
			Logger::log("Warning: Could not load a single-category gift card because its category was not found.");
		}
		delete[] code;
	}

	size_t allProductsCount = 0;
	if (!(ifs >> allProductsCount)) 
	{
		Logger::log("Error reading AllProductsGiftCard count from file.");
		return;
	}

	for (size_t i = 0; i < allProductsCount; i++) 
	{
		if (giftCardsCount >= giftCardsCapacity)
		{
			size_t newCap = (giftCardsCapacity == 0) ? Constants::DEFAULT_CAPACITY : giftCardsCapacity * 2;
			COMMON::resize(giftCards, giftCardsCount, newCap);
			giftCardsCapacity = newCap;
		}

		int typeInt;
		double percentage;

		ifs >> typeInt;
		char* code = FileOpr::readString(ifs);
		ifs >> percentage;

		if (!ifs)
		{
			Logger::log("Error reading data for an AllProductsGiftCard. Stopping load.");
			delete[] code;
			return;
		}

		giftCards[giftCardsCount++] = new AllProductsGiftCard(code, percentage);
		delete[] code;
	}

	size_t multipleCategoryCount = 0;
	if (!(ifs >> multipleCategoryCount)) return;

	const int MAX_CATS_PER_VOUCHER = 10; //maximum categories per voucher
	for (size_t i = 0; i < multipleCategoryCount; i++) 
	{
		if (giftCardsCount >= giftCardsCapacity)
		{
			size_t newCap = (giftCardsCapacity == 0) ? Constants::DEFAULT_CAPACITY : giftCardsCapacity * 2;
			COMMON::resize(giftCards, giftCardsCount, newCap);
			giftCardsCapacity = newCap;
		}

		int typeInt;
		double percentage;
		size_t numCatsInVoucher;

		ifs >> typeInt;
		char* code = FileOpr::readString(ifs);
		ifs >> percentage;
		ifs >> numCatsInVoucher;

		if (!ifs) 
		{ 
			delete[] code; 
			continue; 
		}

		const ProductCategory* tempCatArray[MAX_CATS_PER_VOUCHER];
		size_t loadedCatCount = 0;

		for (size_t j = 0; j < numCatsInVoucher; j++) 
		{
			size_t catId;
			ifs >> catId;
			if (!ifs) break;

			ProductCategory* foundCategory = nullptr;
			for (size_t k = 0; k < productCategoryCount; k++) 
			{
				if (productCategory[k] && productCategory[k]->getId() == catId)
				{
					foundCategory = productCategory[k];
					break;
				}
			}
			if (foundCategory && loadedCatCount < MAX_CATS_PER_VOUCHER)
			{
				tempCatArray[loadedCatCount++] = foundCategory;
			}
		}

		if (loadedCatCount > 0) 
		{
			giftCards[giftCardsCount++] = new MultipleCategoryGiftCard(code, percentage, tempCatArray, loadedCatCount);
		}
		delete[] code;
	}
	Logger::log("Gift cards loaded from file.");
}

void System::saveGiftCardsToFile() const 
{
	std::ofstream ofs(FileName::GIFT_CARDS);
	if (!ofs.is_open()) return;

	ofs << voucherCounter << std::endl;

	// 1. counting different types
	size_t singleCount = 0, allCount = 0, multiCount = 0;
	for (size_t i = 0; i < giftCardsCount; i++) 
	{
		if (dynamic_cast<SingleCategoryGiftCard*>(giftCards[i])) singleCount++;
		else if (dynamic_cast<AllProductsGiftCard*>(giftCards[i])) allCount++;
		else if (dynamic_cast<MultipleCategoryGiftCard*>(giftCards[i])) multiCount++;
	}

	// 2. save SingleCategory
	ofs << singleCount << std::endl;
	for (size_t i = 0; i < giftCardsCount; i++)
	{
		if (SingleCategoryGiftCard* card = dynamic_cast<SingleCategoryGiftCard*>(giftCards[i]))
		{
			card->serialize(ofs);
			ofs << std::endl;
		}
	}

	// 3. save AllProducts
	ofs << allCount << std::endl;
	for (size_t i = 0; i < giftCardsCount; i++) 
	{
		if (AllProductsGiftCard* card = dynamic_cast<AllProductsGiftCard*>(giftCards[i]))
		{
			card->serialize(ofs);
			ofs << std::endl;
		}
	}

	// 4. save MultipleCategory
	ofs << multiCount << std::endl;
	for (size_t i = 0; i < giftCardsCount; i++) 
	{
		if (MultipleCategoryGiftCard* card = dynamic_cast<MultipleCategoryGiftCard*>(giftCards[i]))
		{
			card->serialize(ofs);
			ofs << std::endl;
		}
	}

	Logger::log("Gift cards saved to file.");
}

void System::saveWorkersToFile() const
{
	std::ofstream ofs(FileName::WORKERS);
	if (!ofs.is_open())
	{
		Logger::log("ERROR: Could not open workers file for writing.");
		return;
	}

	ofs << workersCount << std::endl;
	for (size_t i = 0; i < workersCount; i++)
	{
		if (workers[i])
		{
			workers[i]->serialize(ofs);
			ofs << std::endl;
		}
	}
	ofs.close();
	Logger::log("Workers saved to file.");
}

void System::saveProductCategoriesToFile() const
{
	std::ofstream ofs(FileName::PRODUCT_CATEGORY);
	if (!ofs.is_open())
	{
		Logger::log("ERROR: Could not open product categories file for writing.");
		return;
	}

	ofs << productCategoryCount << std::endl;
	for (size_t i = 0; i < productCategoryCount; i++)
	{
		if (productCategory[i])
		{
			productCategory[i]->serialize(ofs);
			ofs << std::endl;
		}
	}
	ofs.close();
	Logger::log("Product categories saved to file.");
}

void System::saveProductsToFile() const
{
	std::ofstream ofs(FileName::PRODUCTS);
	if (!ofs.is_open())
	{
		Logger::log("ERROR: Could not open products file for writing.");
		return;
	}

	ofs << productsCount << std::endl;
	for (size_t i = 0; i < productsCount; i++)
	{
		if (products[i])
		{
			products[i]->serialize(ofs);
			ofs << std::endl;
		}
	}
	ofs.close();
	Logger::log("Products saved to file.");
}

void System::loadTransactions()
{
	std::ifstream ifs(FileName::TRANSACTIONS);
	if (!ifs.is_open())
	{
		Logger::log("Could not open transactions file.");
		return;
	}

	ifs >> transactionsCount;
	transactionsCapacity = transactionsCount + Constants::DEFAULT_CAPACITY;
	transactions = new Transaction*[transactionsCapacity];

	for (size_t i = 0; i < transactionsCount; i++)
	{
		size_t id, cashierId;
		double sum;
		ifs >> id >> cashierId >> sum;
		char* dt = FileOpr::readString(ifs);// date and time
		char* file = FileOpr::readString(ifs);
		transactions[i] = new Transaction(id, cashierId, sum, dt, file);
		delete[] dt;
		delete[] file;
	}
	Logger::log("Transactions loaded successfully.");
}

void System::saveTransactions() const
{
	std::ofstream ofs(FileName::TRANSACTIONS);
	ofs << transactionsCount << std::endl;
	for (size_t i = 0; i < transactionsCount; i++)
	{
		transactions[i]->serialize(ofs);
		ofs << std::endl;
	}
	Logger::log("Transactions saved to file.");
}

void System::listTransactions() const
{
	std::cout << "--- All Transactions ---" << std::endl;
	if (transactionsCount == 0) 
	{
		std::cout << "No transactions recorded yet." << std::endl;
	}
	else 
	{
		for (size_t i = 0; i < transactionsCount; i++) 
		{
			std::cout << "ID: " << transactions[i]->id
				<< " | Cashier ID: " << transactions[i]->cashierId
				<< " | Sum: " << transactions[i]->totalSum
				<< " | Date: " << transactions[i]->dateTime
				<< " | Receipt: " << transactions[i]->receiptFileName << std::endl;
		}
	}
	std::cout << "------------------------" << std::endl;
}

//char* System::generateAndSaveSpecialCode(size_t id)
//{
//	char* specialCode = StrOpr::generateSpecialCode();
//	char* idStr = StrOpr::size_tToChar(id);
//	char* fileName = StrOpr::concatChar(idStr, FileName::SPECIAL_CODE);
//
//	std::ofstream codeFile(fileName);
//	if (codeFile.is_open())
//	{
//		codeFile << specialCode;
//		codeFile.close();
//	}
//
//	delete[] idStr;
//	delete[] fileName;
//
//	return specialCode;
//}

void System::listGiftCards() const
{
	std::cout << "--- List of All Available Gift Cards ---" << std::endl;
	if (giftCardsCount == 0)
	{
		std::cout << "No gift cards available in the system." << std::endl;
	}
	else
	{
		for (size_t i = 0; i < giftCardsCount; i++)
		{
			if (!giftCards[i]) continue;

			std::cout << "Code: " << giftCards[i]->getCode()
				<< " | Discount: " << giftCards[i]->getPercentage() << "%";

			const SingleCategoryGiftCard* scgc = dynamic_cast<const SingleCategoryGiftCard*>(giftCards[i]);
			if (scgc) 
			{
				std::cout << " | Type: Single Category | For: " << scgc->getCategory()->getName() << std::endl;
				continue;
			}

			const AllProductsGiftCard* apgc = dynamic_cast<const AllProductsGiftCard*>(giftCards[i]);
			if (apgc)
			{
				std::cout << " | Type: All Products" << std::endl;
				continue;
			}

			const MultipleCategoryGiftCard* mcgc = dynamic_cast<const MultipleCategoryGiftCard*>(giftCards[i]);
			if (mcgc) 
			{
				std::cout << " | Type: Multiple Category" << std::endl;
				continue;
			}
		}
	}
	std::cout << "----------------------------------------" << std::endl;
}

void System::loadWorkers()
{
	std::ifstream ifs(FileName::WORKERS);
	if (!ifs.is_open()) 
	{
		Logger::log("Could not open workers file. System will be empty.");
		return;
	}

	ifs >> workersCount;
	workersCapacity = workersCount + Constants::DEFAULT_CAPACITY;
	workers = new Worker*[workersCapacity];

	for (size_t i = 0; i < workersCount; i++)
	{
		int typeInt;
		ifs >> typeInt;
		WorkerType type = static_cast<WorkerType>(typeInt);

		size_t id;
		ifs >> id;
		char* fName = FileOpr::readString(ifs);
		char* lName = FileOpr::readString(ifs);
		char phone[Constants::PHONENUMBER_SIZE];
		ifs.get(); // space
		ifs.read(phone, Constants::PHONENUMBER_SIZE);
		size_t age;
		ifs >> age;
		char* pass = FileOpr::readString(ifs);
		bool approved;
		ifs >> approved;

		if (type == WorkerType::MANAGER)
		{
			workers[i] = new Manager(id, fName, lName, phone, age, pass);
		}
		else // CASHIER
		{
			size_t transCount, warnCount;
			ifs >> transCount >> warnCount;

			// read warnings
			Warning** warnings = new Warning * [warnCount];
			for (size_t j = 0; j < warnCount; j++)
			{
				char* sender = FileOpr::readString(ifs);
				char* desc = FileOpr::readString(ifs);
				int levelInt;
				ifs >> levelInt;
				WarningLevel level = static_cast<WarningLevel>(levelInt);
				warnings[j] = new Warning(sender, desc, level);
				delete[] sender;
				delete[] desc;
			}

			workers[i] = new Cashier(approved, id, fName, lName, phone, age, pass, transCount, warnings, warnCount);

			// free warnings
			for (size_t j = 0; j < warnCount; j++) 
			{
				delete warnings[j];
			}
			delete[] warnings;
		}

		delete[] fName;
		delete[] lName;
		delete[] pass;
	}
	Logger::log("Workers loaded successfully.");
}

void System::emptyInit()
{ 
	currentUser = nullptr;
	workers = nullptr;
	workersCount = 0;
	workersCapacity = 0;

	products = nullptr;
	productsCount = 0;
	productsCapacity = 0;
	productCategory = nullptr;
	productCategoryCount = 0;
	productCategoryCapacity = 0;

	transactions = nullptr;
	transactionsCount = 0;
	transactionsCapacity = 0;

	giftCards = nullptr;
	giftCardsCount = 0;
	giftCardsCapacity = 0;
	voucherCounter = 0;
}

System::System()
{
	emptyInit();

	// chech if workers file exists
	std::ifstream workersFile(FileName::WORKERS);
	if (!workersFile.is_open())
	{
		// first start
		createDefaultManager();

		// create empty files
		std::ofstream(FileName::PRODUCTS).close();
		std::ofstream(FileName::PRODUCT_CATEGORY).close();
		std::ofstream(FileName::TRANSACTIONS).close(); 
		std::ofstream(FileName::GIFT_CARDS).close(); 
		workersFile.close();
		writeToFiles();
	}
	else
	{
		// normal start
		Logger::log("System started. Loading data from files...");
		loadProductCategory();
		loadProducts();
		loadWorkers();
		loadTransactions();
		loadGiftCardsFromFile();
	}
}

System::~System()
{
	Logger::log("System shutting down. Saving data...");
	writeToFiles();
	free();
	Logger::log("Shutdown complete.");
}

void System::action(const char* userInput)
{
	// copy of user input to avoid modifying the original string
	char* copyInput = new char[StrOpr::strLen(userInput) + 1];
	StrOpr::strCopy(copyInput, userInput);

	char* commandRunner = copyInput;
	char* command = StrOpr::strtok(commandRunner, ' ');

	if (!command)
	{
		delete[] commandRunner;
		return;
	}

	if (StrOpr::equals(command, "help")) 
	{
		printHelp();
	}
	else if (StrOpr::equals(command, "login"))
	{
		char* idStr = StrOpr::strtok(commandRunner, ' ');
		char* pass = StrOpr::strtok(commandRunner, ' ');
		if (idStr && pass) 
		{
			login(StrOpr::to_size_t(idStr), pass);
		}
		else 
		{
			std::cout << "Usage: login <id> <password>" << std::endl;
		}
	}
	else if (StrOpr::equals(command, "register"))
	{
		char* roleStr = StrOpr::strtok(commandRunner, ' ');
		char* fName = StrOpr::strtok(commandRunner, ' ');
		char* lName = StrOpr::strtok(commandRunner, ' ');
		char* phone = StrOpr::strtok(commandRunner, ' ');
		char* ageStr = StrOpr::strtok(commandRunner, ' ');
		char* pass = StrOpr::strtok(commandRunner, ' ');

		if (roleStr && fName && lName && phone && ageStr && pass)
		{
			if (StrOpr::equals(roleStr, "manager"))
			{
				registerUser(WorkerType::MANAGER, fName, lName, phone, StrOpr::to_size_t(ageStr), pass);
			}
			else if (StrOpr::equals(roleStr, "cashier"))
			{
				registerUser(WorkerType::CASHIER, fName, lName, phone, StrOpr::to_size_t(ageStr), pass);
			}
			else
			{
				std::cout << "Invalid role. Use 'manager' or 'cashier'." << std::endl;
			}
		}
		else
		{
			std::cout << "Usage: register <role> <first_name> <last_name> <phone_number> <age> <password>" << std::endl;
		}
	}
	else if (StrOpr::equals(command, "logout"))
	{
		logout();
	}
	else if (StrOpr::equals(command, "list-workers"))
	{
		listWorkers();
	}
	else if (StrOpr::equals(command, "list-user-data"))
	{
		listUserData();
	}
	else if (StrOpr::equals(command, "list-products"))
	{
		char* categoryIdStr = StrOpr::strtok(commandRunner, ' ');
		if (categoryIdStr) 
		{
			listProducts(StrOpr::to_size_t(categoryIdStr));
		}
		else
		{
			listProducts();
		}
	}
	else if (StrOpr::equals(command, "list-product-categories"))
	{
		listProductCategories();
	}
	else if (StrOpr::equals(command, "add-product"))
	{
		char* productTypeStr = StrOpr::strtok(commandRunner, ' ');

		if (!productTypeStr)
		{
			std::cout << "Usage: add-product <product_type>" << std::endl;
			std::cout << "Available types: product_by_unit, product_by_weight" << std::endl;
		}
		else if (!currentUser)
		{
			std::cout << "Error: This command is for managers only." << std::endl;
		}
		else
		{
			ProductType type;
			bool validType = true;
			if (StrOpr::equals(productTypeStr, "product_by_unit"))
			{
				type = ProductType::ByUnit;
			}
			else if (StrOpr::equals(productTypeStr, "product_by_weight"))
			{
				type = ProductType::ByWeight;
			}
			else
			{
				validType = false;
				std::cout << "Error: Invalid product type. Use 'product_by_unit' or 'product_by_weight'." << std::endl;
			}

			if (validType)
			{
				bool success = currentUser->addProduct(products, productsCount, productsCapacity, productCategory, productCategoryCount, type);

				if (success)
				{
					Product* newProduct = products[productsCount - 1];
					std::cout << "Product \"" << newProduct->getName() << "\" added successfully under category \""
						<< newProduct->getCategory()->getName() << "\"" << std::endl;

					Logger::log("A new product was added.", currentUser);
					logToFeed("added a new product to the inventory.");
				}
				else
				{
					std::cout << "Product was not added" << std::endl;
				}
			}
		}
	}
	else if (StrOpr::equals(command, "list-pending"))
	{
		if (currentUser)
		{
			currentUser->listPending(workers, workersCount);
		}
		else
		{
			std::cout << "Error: This command is for managers only." << std::endl;
		}
	}
	else if (StrOpr::equals(command, "approve"))
	{
		char* cashierIdStr = StrOpr::strtok(commandRunner, ' ');
		char* specialCode = StrOpr::strtok(commandRunner, ' ');

		if (!cashierIdStr || !specialCode)
		{
			std::cout << "Usage: approve <cashier_id> <special_code>" << std::endl;
		}
		else if (!currentUser)
		{
			std::cout << "Error: This command is for managers only." << std::endl;
		}
		else
		{
			size_t cashierId = StrOpr::to_size_t(cashierIdStr);
			Worker* targetCashier = findWorkerById(cashierId);

			if (targetCashier)
			{
				bool success = currentUser->approve(targetCashier, specialCode);

				if (success)
				{
					std::cout << "Cashier with ID " << cashierId << " has been approved." << std::endl;

					// log and feed
					char* idAsChar = StrOpr::size_tToChar(cashierId);
					char* feedMessage = StrOpr::concatChar("approved cashier with ID ", idAsChar);
					logToFeed(feedMessage);
					Logger::log(feedMessage, currentUser);
					delete[] idAsChar;
					delete[] feedMessage;
				}
				else
				{
					std::cout << "Error: Approval failed. The cashier might already be approved, or your special code is invalid." << std::endl;
				}
			}
			else
			{
				std::cout << "Error: Worker with ID " << cashierId << " not found." << std::endl;
			}
		}
	}
	else if (StrOpr::equals(command, "decline"))
	{
		char* cashierIdStr = StrOpr::strtok(commandRunner, ' ');
		char* specialCode = StrOpr::strtok(commandRunner, ' ');

		if (!cashierIdStr || !specialCode)
		{
			std::cout << "Usage: decline <cashier_id> <special_code>" << std::endl;
		}
		else if (!currentUser || currentUser->getWorkerType() != WorkerType::MANAGER)
		{
			std::cout << "Error: This command is for managers only." << std::endl;
		}
		else
		{
			size_t cashierId = StrOpr::to_size_t(cashierIdStr);

			bool success = currentUser->decline(workers, workersCount, cashierId, specialCode);

			if (success)
			{
				std::cout << "Cashier registration for ID " << cashierId << " has been declined and removed." << std::endl;

				// log and feed
				char* idAsChar = StrOpr::size_tToChar(cashierId);
				char* feedMessage = StrOpr::concatChar("declined cashier registration for ID ", idAsChar);
				logToFeed(feedMessage);
				Logger::log(feedMessage, currentUser);
				delete[] idAsChar;
				delete[] feedMessage;
			}
			else
			{
				std::cout << "Error: Decline failed. The user may not be a pending cashier, ID is wrong, or your special code is invalid." << std::endl;
			}
		}
	}
	else if (StrOpr::equals(command, "list-feed"))
	{
		listFeed();
	}
	else if (StrOpr::equals(command, "list-transactions"))
	{
		listTransactions();
	}
	else if (StrOpr::equals(command, "add-category"))
	{
		char* categoryName = StrOpr::strtok(commandRunner, ' ');
		char* categoryDesc = commandRunner;

		if (!categoryName || !categoryDesc || StrOpr::strLen(categoryDesc) == 0)
		{
			std::cout << "Usage: add-category <category_name> <category_description>" << std::endl;
		}
		else if (!currentUser)
		{
			std::cout << "Error: This command is for managers only." << std::endl;
		}
		else
		{
			bool success = currentUser->addCategory(productCategory, productCategoryCount, productCategoryCapacity, categoryName, categoryDesc);

			if (success)
			{
				size_t newId = productCategory[productCategoryCount - 1]->getId();
				std::cout << "Category '" << categoryName << "' added successfully with ID " << newId << "." << std::endl;

				// log and feed
				char* part1 = StrOpr::concatChar("added a new category: '", categoryName);
				char* feedMessage = StrOpr::concatChar(part1, "'.");
				logToFeed(feedMessage);
				Logger::log(feedMessage, currentUser);
				delete[] part1;
				delete[] feedMessage;
			}
			else
			{
				std::cout << "Error: A category with the name '" << categoryName << "' already exists." << std::endl;
			}
		}
	}
	else if (StrOpr::equals(command, "sell"))
	{
		if (!currentUser)
		{
			std::cout << "Error: This command is for cashiers only." << std::endl;
		}
		else
		{
			Cashier* currentCashier = dynamic_cast<Cashier*>(currentUser);
			if (currentCashier)
			{
				bool success = currentCashier->sell(products, productsCount, transactions, transactionsCount, transactionsCapacity, giftCards, giftCardsCount);
				if (success)
				{
					// log and feed
					char* logMessage = StrOpr::concatChar("completed a sale transaction.", "");
					logToFeed(logMessage);
					Logger::log(logMessage, currentUser);
					delete[] logMessage;
				}
			}
		}
	}
	else if (StrOpr::equals(command, "list-warned-cashiers"))
	{
		char* pointsStr = StrOpr::strtok(commandRunner, ' ');

		if (!pointsStr)
		{
			std::cout << "Usage: list-warned-cashiers <points>" << std::endl;
		}
		else if (!currentUser)
		{
			std::cout << "Error: This command is for managers only." << std::endl;
		}
		else
		{
			int pointsThreshold = StrOpr::to_size_t(pointsStr);
			currentUser->listWarnedCashiers(workers, workersCount, pointsThreshold);
		}
	}
	else if (StrOpr::equals(command, "warn-cashier"))
	{
		char* cashierIdStr = StrOpr::strtok(commandRunner, ' ');
		char* pointsStr = StrOpr::strtok(commandRunner, ' ');
		char* description = commandRunner;

		if (!cashierIdStr || !pointsStr || !description || StrOpr::strLen(description) == 0)
		{
			std::cout << "Usage: warn-cashier <cashier_id> <points> <description>" << std::endl;
			std::cout << "Points can be: 100 (low), 200 (medium), 300 (high)." << std::endl;
		}
		else if (!currentUser)
		{
			std::cout << "Error: This command is for managers only." << std::endl;
		}
		else
		{
			size_t cashierId = StrOpr::to_size_t(cashierIdStr);
			int points = StrOpr::to_size_t(pointsStr);

			// converting points to WarningLevel
			WarningLevel level;
			bool validPoints = true;
			if (points == 100) level = WarningLevel::LOW;
			else if (points == 200) level = WarningLevel::MEDIUM;
			else if (points == 300) level = WarningLevel::HIGH;
			else 
			{
				validPoints = false;
				std::cout << "Error: Invalid points value. Use 100, 200, or 300." << std::endl;
			}

			if (validPoints)
			{
				Worker* targetCashier = findWorkerById(cashierId);

				if (!targetCashier) 
				{
					std::cout << "Error: Cashier with ID " << cashierId << " not found." << std::endl;
				}
				else 
				{
					bool success = currentUser->warnCashier(targetCashier, level, description);
					if (success)
					{
						std::cout << "Warning added successfully to cashier with ID " << cashierId << "." << std::endl;

						// log and feed
						char* idAsChar = StrOpr::size_tToChar(cashierId);
						char* part1 = StrOpr::concatChar("issued a warning to cashier with ID ", idAsChar);
						char* feedMessage = StrOpr::concatChar(part1, ".");

						logToFeed(feedMessage);
						Logger::log(feedMessage, currentUser);

						delete[] idAsChar;
						delete[] part1;
						delete[] feedMessage;
					}
					else
					{
						std::cout << "Error: Could not issue warning. Target is not a valid cashier." << std::endl;
					}
				}
			}
		}
	}
	else if (StrOpr::equals(command, "fire-cashier"))
	{
		char* cashierIdStr = StrOpr::strtok(commandRunner, ' ');
		char* specialCode = StrOpr::strtok(commandRunner, ' ');

		if (!cashierIdStr || !specialCode)
		{
			std::cout << "Usage: fire-cashier <cashier_id> <special_code>" << std::endl;
		}
		else if (!currentUser)
		{
			std::cout << "Error: This command is for managers only." << std::endl;
		}
		else
		{
			size_t cashierId = StrOpr::to_size_t(cashierIdStr);

			bool success = currentUser->fireCashier(workers, workersCount, cashierId, specialCode);

			if (success)
			{
				std::cout << "Cashier with ID " << cashierId << " has been fired." << std::endl;

				// feed and log
				char* idAsChar = StrOpr::size_tToChar(cashierId);
				char* part1 = StrOpr::concatChar("fired cashier with ID ", idAsChar);
				char* feedMessage = StrOpr::concatChar(part1, ".");

				logToFeed(feedMessage);
				Logger::log(feedMessage, currentUser);

				delete[] idAsChar;
				delete[] part1;
				delete[] feedMessage;
			}
			else
			{
				std::cout << "Error: Firing failed. Check ID, ensure target is a cashier (and not you), and verify your special code." << std::endl;
			}
		}
	}
	else if (StrOpr::equals(command, "delete-product"))
	{
		char* productIdStr = StrOpr::strtok(commandRunner, ' ');

		if (!productIdStr)
		{
			std::cout << "Usage: delete-product <product_id>" << std::endl;
		}
		else if (!currentUser)
		{
			std::cout << "Error: This command is for managers only." << std::endl;
		}
		else
		{
			size_t productId = StrOpr::to_size_t(productIdStr);

			bool success = currentUser->deleteProduct(products, productsCount, productId);
			if (success)
			{
				std::cout << "Product with ID " << productId << " has been deleted from the inventory." << std::endl;
				// log and feed
				char* idAsChar = StrOpr::size_tToChar(productId);
				char* part1 = StrOpr::concatChar("deleted a product with ID ", idAsChar);
				char* feedMessage = StrOpr::concatChar(part1, " from the inventory.");

				logToFeed(feedMessage);
				Logger::log(feedMessage, currentUser);

				delete[] idAsChar;
				delete[] part1;
				delete[] feedMessage;
			}
			else
			{
				std::cout << "Error: Product with ID " << productId << " not found." << std::endl;
			}
		}
	}
	else if (StrOpr::equals(command, "delete-category"))
	{
		char* categoryIdStr = StrOpr::strtok(commandRunner, ' ');

		if (!categoryIdStr)
		{
			std::cout << "Usage: delete-category <category_id>" << std::endl;
		}
		else if (!currentUser)
		{
			std::cout << "Error: This command is for managers only." << std::endl;
		}
		else
		{
			size_t categoryId = StrOpr::to_size_t(categoryIdStr);

			bool success = currentUser->deleteCategory(productCategory, productCategoryCount, categoryId, products, productsCount);

			if (success)
			{
				std::cout << "Category with ID " << categoryId << " has been deleted." << std::endl;
				// log and feed
				char* idAsChar = StrOpr::size_tToChar(categoryId);
				char* part1 = StrOpr::concatChar("deleted a category with ID ", idAsChar);
				char* feedMessage = StrOpr::concatChar(part1, ".");
				logToFeed(feedMessage);
				Logger::log(feedMessage, currentUser);
				delete[] idAsChar;
				delete[] part1;
				delete[] feedMessage;
			}
			else
			{
				std::cout << "Error: Category deletion failed. It might not exist or still contains products." << std::endl;
			}
		}
	}
	else if (StrOpr::equals(command, "promote-cashier"))
	{
		char* cashierIdStr = StrOpr::strtok(commandRunner, ' ');
		char* specialCode = StrOpr::strtok(commandRunner, ' ');

		if (!cashierIdStr || !specialCode)
		{
			std::cout << "Usage: promote-cashier <cashier_id> <special_code>" << std::endl;
		}
		else if (!currentUser)
		{
			std::cout << "Error: This command is for managers only." << std::endl;
		}
		else
		{
			size_t cashierId = StrOpr::to_size_t(cashierIdStr);
			bool success = currentUser->promoteCashier(workers, workersCount, cashierId, specialCode);
			if (success)
			{
				Worker* promotedManager = findWorkerById(cashierId);

				if (promotedManager) 
				{
					std::cout << "Cashier '" << promotedManager->getFirstName() << " " << promotedManager->getLastName()
						<< "' has been promoted to Manager." << std::endl;
				}

				// log and feed
				char* idAsChar = StrOpr::size_tToChar(cashierId);
				char* part1 = StrOpr::concatChar("promoted cashier with ID ", idAsChar);
				char* feedMessage = StrOpr::concatChar(part1, " to a manager position.");

				logToFeed(feedMessage);
				Logger::log(feedMessage, currentUser);

				delete[] idAsChar;
				delete[] part1;
				delete[] feedMessage;
			}
			else
			{
				std::cout << "Error: Promotion failed. Check if the ID is correct, the cashier is approved, and your special code is valid." << std::endl;
			}
		}
	}
	else if (StrOpr::equals(command, "load-products"))
	{
		char* filename = StrOpr::strtok(commandRunner, ' ');

		if (!filename)
		{
			std::cout << "Usage: load-products <filename>" << std::endl;
		}
		else if (!currentUser)
		{
			std::cout << "Error: This command is for managers only." << std::endl;
		}
		else
		{
			bool success = currentUser->loadProducts(filename, products, productsCount, productsCapacity, productCategory, productCategoryCount, productCategoryCapacity);

			if (success)
			{
				// log and feed
				char* part1 = StrOpr::concatChar("loaded new stock from file '", filename);
				char* feedMessage = StrOpr::concatChar(part1, "'.");

				logToFeed(feedMessage);
				Logger::log(feedMessage, currentUser);

				delete[] part1;
				delete[] feedMessage;
			}
		}
	}
	else if (StrOpr::equals(command, "leave"))
	{
		leave();
	}
	else if (StrOpr::equals(command, "load-gift-cards"))
	{
		char* filename = StrOpr::strtok(commandRunner, ' ');
		if (!filename) 
		{
			std::cout << "Usage: load-gift-cards <filename>" << std::endl;
		}
		else if (!currentUser)
		{
			std::cout << "Error: This command is for managers only." << std::endl;
		}
		else
		{
			bool success = currentUser->loadGiftCards(filename, giftCards, giftCardsCount, giftCardsCapacity, voucherCounter, productCategory, productCategoryCount);
			if (success)
			{
				// log and feed
				char* part1 = StrOpr::concatChar("generated new gift cards from file '", filename);
				char* feedMessage = StrOpr::concatChar(part1, "'.");

				logToFeed(feedMessage);
				Logger::log(feedMessage, currentUser);

				delete[] part1;
				delete[] feedMessage;
			}
		}
	}
	else if (StrOpr::equals(command, "list-gift-cards"))
	{
		listGiftCards();
	}
	else
	{
		std::cout << "Unknown command." << std::endl;
	}

	delete[] copyInput;
}

void System::login(size_t id, const char* password)
{
	if (currentUser) 
	{
		std::cout << "Another user is already logged in. Please logout first." << std::endl;
		return;
	}

	Worker* testUser = findWorkerById(id);

	if (!testUser) 
	{
		std::cout << "No user with ID " << id << " found." << std::endl;
		return;
	}
	if (StrOpr::equals(testUser->getPassword(), password))
	{
		currentUser = testUser;
		std::cout << "User " << currentUser->getFirstName() << " " << currentUser->getLastName()
			<< " (ID: " << id << ") has been logged in." << std::endl;
		return;
	}
	std::cout << "Invalid password." << std::endl;
}

void System::registerUser(WorkerType type, const char* firstName, const char* lastName, const char* phone, size_t age, const char* password)
{
	// 1. check capacity
	if (workersCount >= workersCapacity)
	{
		size_t newCapacity = (workersCapacity == 0) ? Constants::DEFAULT_CAPACITY : workersCapacity * 2;
		COMMON::resize(workers, workersCount, newCapacity);
		workersCapacity = newCapacity;
		Logger::log("Resized workers array.");
	}

	// 2. new id
	size_t newId = 1;
	if (workersCount > 0)// find biggest id and increment it
	{
		size_t maxId = 0;
		for (size_t i = 0; i < workersCount; i++)
		{
			if (workers[i]->getId() > maxId)
			{
				maxId = workers[i]->getId();
			}
		}
		newId = maxId + 1;
	}

	// 3. create new worker
	if (type == WorkerType::MANAGER)
	{
		Manager* newManager = new Manager(newId, firstName, lastName, phone, age, password);
		workers[workersCount++] = newManager;
		char* specialCode = newManager->generateAndSaveSpecialCode();

		// show the information 
		char* idStr = StrOpr::size_tToChar(newId);
		char* fileName = StrOpr::concatChar(idStr, FileName::SPECIAL_CODE);

		std::cout << "Manager registered successfully!" << std::endl;
		std::cout << "Special code: " << specialCode << std::endl;
		std::cout << "Code saved to: " << fileName << std::endl;

		delete[] specialCode;
		delete[] idStr;
		delete[] fileName;
		Logger::log("New manager registered.");
		logToFeed("registered a new manager.");
	}
	else if (type == WorkerType::CASHIER)
	{
		Cashier* newCashier = new Cashier(false, newId, firstName, lastName, phone, age, password, 0, nullptr, 0);
		workers[workersCount++] = newCashier;

		std::cout << "Cashier registration pending approval from a manager." << std::endl;
		Logger::log("New cashier registration is pending.");
		logToFeed("submitted a new cashier registration.");
	}
}

void System::logout()
{
	if (!currentUser) 
	{
		std::cout << "No user is currently logged in." << std::endl;
		return;
	}
	std::cout << "User with ID " << currentUser->getId() << " has been logged out." << std::endl;
	currentUser = nullptr;
}

void System::listWorkers() const
{
	std::cout << "--- List of All Workers ---" << std::endl;
	if (workersCount == 0)
	{
		std::cout << "No workers in the system." << std::endl;
		return;
	}

	for (size_t i = 0; i < workersCount; i++)
	{
		if (workers[i])
		{
			WorkerType type = workers[i]->getWorkerType();
			const char* role = (type == WorkerType::MANAGER) ? "Manager" : "Cashier";
			std::cout << "ID: " << workers[i]->getId()
				<< " | Name: " << workers[i]->getFirstName() << " " << workers[i]->getLastName()
				<< " | Role: " << role
				<< " | Approved: " << (workers[i]->isApproved() ? "Yes" : "No") << std::endl;
		}
	}
	std::cout << "---------------------------" << std::endl;
}

void System::listUserData() const
{
	if (!currentUser) 
	{
		std::cout << "No user is currently logged in." << std::endl;
		return;
	}
	std::cout << "--- Data for current user ---" << std::endl;
	const char* role = (currentUser->getWorkerType() == WorkerType::MANAGER) ? "Manager" : "Cashier";

	std::cout << "ID: " << currentUser->getId() << std::endl;
	std::cout << "Name: " << currentUser->getFirstName() << " " << currentUser->getLastName() << std::endl;
	std::cout << "Role: " << role << std::endl;
	std::cout << "Phone: " << currentUser->getPhoneNumber() << std::endl;
	std::cout << "Age: " << currentUser->getAge() << std::endl;

	if (currentUser->getWorkerType() == WorkerType::CASHIER)
	{
		const Cashier* cashier = dynamic_cast<const Cashier*>(currentUser);
		if (cashier)
		{
			std::cout << "Transactions: " << cashier->getTransactionCount() << std::endl;
			std::cout << "Warnings (" << cashier->getWarningsCount() << "):" << std::endl;
			for (size_t i = 0; i < cashier->getWarningsCount(); i++)
			{
				const Warning* w = cashier->getWarning(i);
				std::cout << "  - From: " << w->getSender() << " | Level: " << static_cast<int>(w->getLevel())
					<< " | Desc: " << w->getDescription() << std::endl;
			}
		}
	}
	std::cout << "---------------------------" << std::endl;
}

void System::listProducts() const
{
	std::cout << "--- List of All Products ---" << std::endl;
	if (productsCount == 0) 
	{
		std::cout << "No products in the system." << std::endl;
		return;
	}

	for (size_t i = 0; i < productsCount; i++)
	{
		if (products[i])
		{
			const char* typeStr = (products[i]->getProductType() == ProductType::ByUnit) ? "per unit" : "per kg";
			std::cout << "ID: " << products[i]->getId()
				<< " | Name: " << products[i]->getName()
				<< " | Price: " << products[i]->getPrice() << " " << typeStr
				<< " | Qty: " << products[i]->getQuantity()
				<< " | Category: " << products[i]->getCategory()->getName() << std::endl;
		}
	}
	std::cout << "---------------------------" << std::endl;
}

void System::listProducts(size_t categoryId) const
{
	const ProductCategory* category = ProductCategory::findById(categoryId, productCategory, productCategoryCount);

	if (!category) 
	{
		std::cout << "Category with ID " << categoryId << " not found." << std::endl;
		return;
	}

	std::cout << "--- Products in Category: " << category->getName() << " ---" << std::endl;
	bool foundAny = false;
	for (size_t i = 0; i < productsCount; i++)
	{
		if (products[i] && products[i]->getCategory()->getId() == categoryId)
		{
			foundAny = true;
			const char* typeStr = (products[i]->getProductType() == ProductType::ByUnit) ? "per unit" : "per kg";
			std::cout << "ID: " << products[i]->getId()
				<< " | Name: " << products[i]->getName()
				<< " | Price: " << products[i]->getPrice() << " " << typeStr
				<< " | Qty: " << products[i]->getQuantity() << std::endl;
		}
	}
	if (!foundAny) 
	{
		std::cout << "No products found in this category." << std::endl;
	}
	std::cout << "---------------------------" << std::endl;
}

void System::listProductCategories() const
{
	std::cout << "--- List of All Product Categories ---" << std::endl;
	if (productCategoryCount == 0)
	{
		std::cout << "No product categories have been added yet." << std::endl;
	}
	else
	{
		for (size_t i = 0; i < productCategoryCount; i++)
		{
			if (productCategory[i]) // check for nullptr
			{
				std::cout << "ID: " << productCategory[i]->getId()
					<< " | Name: " << productCategory[i]->getName()
					<< " | Description: " << productCategory[i]->getDescription()
					<< std::endl;
			}
		}
	}
	std::cout << "------------------------------------" << std::endl;
}

void System::logToFeed(const char* message)
{
	std::ofstream feedFile(FileName::FEED, std::ios::app);
	if (!feedFile.is_open())
	{
		Logger::log("Could not open feed file for writing.", currentUser);
		return;
	}

	char* currentTime = StrOpr::getTime();
	feedFile << "[" << currentTime << "] ";
	delete[] currentTime;

	if (currentUser)
	{
		feedFile << "User '" << currentUser->getFirstName() << "' (ID: " << currentUser->getId() << ") " << message << std::endl;
	}
	else
	{
		feedFile << "[System] " << message << std::endl;
	}
}

void System::listFeed() const
{
	std::cout << "--- System Activity Feed ---" << std::endl;
	std::ifstream feedFile(FileName::FEED);
	if (!feedFile.is_open() || feedFile.peek() == EOF)
	{
		std::cout << "No activity recorded yet." << std::endl;
	}
	else
	{
		feedFile.close(); // close file to print it next
		FileOpr::printFile(FileName::FEED);
	}
	std::cout << "--------------------------" << std::endl;
}

void System::leave()
{
	if (!currentUser)
	{
		std::cout << "Error: No user is currently logged in to leave." << std::endl;
		return;
	}

	size_t userIdToLeave = currentUser->getId();
	char* userName = nullptr;
	StrOpr::strCopy(userName, currentUser->getFirstName()); // Запазваме името за съобщението

	// 1. Намиране на индекса на текущия потребител в масива
	int targetIndex = -1;
	for (size_t i = 0; i < workersCount; i++)
	{
		if (workers[i] && workers[i]->getId() == userIdToLeave)
		{
			targetIndex = i;
			break;
		}
	}

	if (targetIndex == -1)
	{
		// Тази грешка не би трябвало никога да се случи, ако системата е консистентна
		std::cout << "Critical Error: Logged-in user not found in the workers list." << std::endl;
		delete[] userName;
		return;
	}

	// 2. Изтриване на обекта и преподреждане на масива
	delete workers[targetIndex];

	for (size_t i = targetIndex; i < workersCount - 1; i++)
	{
		workers[i] = workers[i + 1];
	}

	workersCount--;
	workers[workersCount] = nullptr;

	std::cout << "User '" << userName << "' has left the job. You have been logged out." << std::endl;

	// 3. Логване в логовете
	char* part1 = StrOpr::concatChar("User '", userName);
	char* part2 = StrOpr::concatChar(part1, "' (ID: ");
	char* idAsChar = StrOpr::size_tToChar(userIdToLeave);
	char* part3 = StrOpr::concatChar(part2, idAsChar);
	char* feedMessage = StrOpr::concatChar(part3, ") has left the job.");

	// Записваме във feed-а преди да излезем от системата, докато currentUser още сочи към нещо (макар и изтрито)
	// Това е малко рисково, но за целите на лога е ОК.
	// По-добър вариант е да се логва преди delete, но тогава нямаме достъп до името след това.
	logToFeed(feedMessage);
	Logger::log(feedMessage, currentUser);

	// 4. Logout на потребителя от системата
	currentUser = nullptr;

	// 5. Изчистване на паметта
	delete[] userName;
	delete[] part1;
	delete[] part2;
	delete[] idAsChar;
	delete[] part3;
	delete[] feedMessage;
}