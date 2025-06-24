#include "Manager.h"
#include "Cashier.h"
#include "Logger.h"
#include "ProductCategory.h"
#include "Product.h"
#include "ProductByUnit.h"
#include "ProductByWeight.h"
#include "CommonFunctions.hpp"
#include "SingleCategoryGiftCard.h"
#include "AllProductsGiftCard.h"
#include "MultipleCategoryGiftCard.h"
#include "Constants.h"
#include <iostream>


bool Manager::validateSpecialCode(const char* specialCode) const
{
	char* idStr = StrOpr::size_tToChar(this->getId());
	char* fileName = StrOpr::concatChar(idStr, FileName::SPECIAL_CODE);
	delete[] idStr; 

	std::ifstream codeFile(fileName);
	if (!codeFile.is_open())
	{
		delete[] fileName; 
		return false; 
	}
	char fileCode[Constants::SPECIAL_CODE_LENGTH + 1];
	codeFile.read(fileCode, Constants::SPECIAL_CODE_LENGTH);
	fileCode[Constants::SPECIAL_CODE_LENGTH] = '\0';
	codeFile.close();

	delete[] fileName;
	return StrOpr::equals(fileCode, specialCode);
}

int Manager::findWorkerIndexById(size_t workerId, Worker** allWorkers, size_t workersCount) const
{
	for (size_t i = 0; i < workersCount; i++)
	{
		if (allWorkers[i] && allWorkers[i]->getId() == workerId)
		{
			return i;
		}
	}

	return -1;
}

int Manager::processAllProductsCard(char*& lineRunner, GiftCard**& giftCards, size_t& giftCardsCount, size_t& giftCardsCapacity, size_t& voucherCounter, int lineNum) const
{
	// Format: AllProducts:<PERCENT>:<COUNT>
	char* percentStr = StrOpr::strtok(lineRunner, ':');
	char* countStr = StrOpr::strtok(lineRunner, ':');
	if (!percentStr || !countStr) 
	{
		std::cout << "Warning: Invalid format for AllProducts on line " << lineNum << ". Skipping." << std::endl;
		return 0;
	}

	double percent = StrOpr::to_double(percentStr);
	int countToGen = StrOpr::to_size_t(countStr);
	int generated = 0;

	for (int i = 0; i < countToGen; i++) 
	{
		voucherCounter++;
		char* newCode = StrOpr::generateVoucherCode(voucherCounter);

		if (giftCardsCount >= giftCardsCapacity)
		{
			size_t newCap = (giftCardsCapacity == 0) ? Constants::DEFAULT_CAPACITY : giftCardsCapacity * 2;
			COMMON::resize(giftCards, giftCardsCount, newCap);
			giftCardsCapacity = newCap;
		}
		giftCards[giftCardsCount++] = new AllProductsGiftCard(newCode, percent);
		delete[] newCode;
		generated++;
	}
	return generated;
}

int Manager::processSingleCategoryCard(char*& lineRunner, GiftCard**& giftCards, size_t& giftCardsCount, size_t& giftCardsCapacity, size_t& voucherCounter, ProductCategory** allCategories, size_t categoryCount, int lineNum) const
{
	// Format: SingleCategory:<CATEGORY_NAME>:<PERCENT>:<COUNT>
	char* categoryName = StrOpr::strtok(lineRunner, ':');
	char* percentStr = StrOpr::strtok(lineRunner, ':');
	char* countStr = StrOpr::strtok(lineRunner, ':');
	if (!categoryName || !percentStr || !countStr) 
	{
		std::cout << "Warning: Invalid format for SingleCategory on line " << lineNum << ". Skipping." << std::endl;
		return 0;
	}

	const ProductCategory* foundCategory = ProductCategory::findByName(categoryName, allCategories, categoryCount);
	if (!foundCategory) 
	{
		std::cout << "Warning: Category '" << categoryName << "' not found on line " << lineNum << ". Skipping." << std::endl;
		return 0;
	}

	double percent = StrOpr::to_double(percentStr);
	int countToGen = StrOpr::to_size_t(countStr);
	int generated = 0;

	for (int i = 0; i < countToGen; i++) 
	{
		voucherCounter++;
		char* newCode = StrOpr::generateVoucherCode(voucherCounter);

		if (giftCardsCount >= giftCardsCapacity) 
		{
			size_t newCap = (giftCardsCapacity == 0) ? Constants::DEFAULT_CAPACITY : giftCardsCapacity * 2;
			COMMON::resize(giftCards, giftCardsCount, newCap);
			giftCardsCapacity = newCap;
		}
		giftCards[giftCardsCount++] = new SingleCategoryGiftCard(newCode, percent, foundCategory);
		delete[] newCode;
		generated++;
	}
	return generated;
}

int Manager::processMultipleCategoryCard(char*& lineRunner, GiftCard**& giftCards, size_t& giftCardsCount, size_t& giftCardsCapacity, size_t& voucherCounter, ProductCategory** allCategories, size_t categoryCount, int lineNum) const
{
	// Format: MultipleCategory:Cat1,Cat2,Cat3:PERCENT:COUNT
	char* categoriesStr = StrOpr::strtok(lineRunner, ':');
	char* percentStr = StrOpr::strtok(lineRunner, ':');
	char* countStr = StrOpr::strtok(lineRunner, ':');
	if (!categoriesStr || !percentStr || !countStr) 
	{
		std::cout << "Warning: Invalid format for MultipleCategory on line " << lineNum << ". Skipping." << std::endl;
		return 0;
	}

	const int MAX_CATS_PER_VOUCHER = 10;
	const ProductCategory* foundCategories[MAX_CATS_PER_VOUCHER];
	size_t foundCount = 0;

	char* catName = StrOpr::strtok(categoriesStr, ',');
	while (catName != nullptr && foundCount < MAX_CATS_PER_VOUCHER)
	{
		const ProductCategory* cat = ProductCategory::findByName(catName, allCategories, categoryCount);
		if (cat) 
		{
			foundCategories[foundCount++] = cat;
		}
		else
		{
			std::cout << "Warning: Category '" << catName << "' not found on line " << lineNum << ". It will be ignored." << std::endl;
		}
		catName = StrOpr::strtok(categoriesStr, ','); // strtok continues on the same string
	}

	if (foundCount == 0)
	{
		std::cout << "Warning: No valid categories found for MultipleCategory voucher on line " << lineNum << ". Skipping." << std::endl;
		return 0;
	}

	double percent = StrOpr::to_double(percentStr);
	int countToGen = StrOpr::to_size_t(countStr);
	int generated = 0;

	for (int i = 0; i < countToGen; i++)
	{
		voucherCounter++;
		char* newCode = StrOpr::generateVoucherCode(voucherCounter);

		if (giftCardsCount >= giftCardsCapacity) 
		{
			size_t newCap = (giftCardsCapacity == 0) ? Constants::DEFAULT_CAPACITY : giftCardsCapacity * 2;
			COMMON::resize(giftCards, giftCardsCount, newCap);
			giftCardsCapacity = newCap;
		}
		giftCards[giftCardsCount++] = new MultipleCategoryGiftCard(newCode, percent, foundCategories, foundCount);
		delete[] newCode;
		generated++;
	}
	return generated;
}

Manager::Manager(size_t id, const char* firstName, const char* lastName, const char* phoneNumber, size_t age, const char* password)
	: Worker(WorkerType::MANAGER, true, id, firstName, lastName, phoneNumber, age, password)
{
}

Manager::Manager(const Manager& other)
	: Worker(other)
{
}

Manager::Manager(Manager&& other) noexcept
	: Worker(std::move(other))
{
}

Manager::~Manager()
{
}

Manager& Manager::operator=(const Manager& other)
{
	if (this != &other)
	{
		Worker::operator=(other);
	}
	return *this;
}

Manager& Manager::operator=(Manager&& other) noexcept
{
	if (this != &other)
	{
		Worker::operator=(std::move(other));
	}
	return *this;
}

void Manager::listPending(Worker** allWorkers, size_t count) const
{
	std::cout << "--- Pending Cashier Registrations ---" << std::endl;
	bool foundPending = false;
	for (size_t i = 0; i < count; i++)
	{
		if (allWorkers[i]->getWorkerType() == WorkerType::CASHIER && !allWorkers[i]->isApproved())
		{
			foundPending = true;
			std::cout << "ID: " << allWorkers[i]->getId()
				<< " | Name: " << allWorkers[i]->getFirstName() << " " << allWorkers[i]->getLastName()
				<< std::endl;
		}
	}

	// no pending
	if (!foundPending)
	{
		std::cout << "No pending registrations found." << std::endl;
	}
	std::cout << "------------------------------------" << std::endl;
}

bool Manager::approve(Worker* cashierToApprove, const char* specialCode) const
{
	// wrong special code
	if (!validateSpecialCode(specialCode))
	{
		return false;
	}

	// not cashier
	if (cashierToApprove->getWorkerType() != WorkerType::CASHIER) 
	{
		return false; 
	}
	// already approved
	if (cashierToApprove->isApproved()) 
	{
		return false; // Грешка: Вече е одобрен
	}

	cashierToApprove->setApproved(true);

	return true; 
}

bool Manager::decline(Worker** allWorkers, size_t& count, size_t cashierIdToDecline, const char* specialCode) const
{
	//wrong special code
	if (!validateSpecialCode(specialCode))
	{
		return false;
	}

	int targetIndex = findWorkerIndexById(cashierIdToDecline, allWorkers, count);

	// no such cashier
	if (targetIndex == -1) 
	{
		return false; // Грешка: Касиерът не е намерен
	}

	// not a cashier or already approved
	if (allWorkers[targetIndex]->getWorkerType() != WorkerType::CASHIER || allWorkers[targetIndex]->isApproved()) 
	{
		return false;
	}

	// remove cashier
	delete allWorkers[targetIndex];
	for (size_t i = targetIndex; i < count - 1; i++) 
	{
		allWorkers[i] = allWorkers[i + 1];
	}
	count--;
	allWorkers[count] = nullptr;

	return true;
}

void Manager::listWarnedCashiers(Worker** allWorkers, size_t workersCount, int pointsThreshold) const
{
	std::cout << "--- Cashiers with more than " << pointsThreshold << " penalty points ---" << std::endl;
	bool foundAny = false;

	for (size_t i = 0; i < workersCount; i++)
	{
		if (allWorkers[i] && allWorkers[i]->getWorkerType() == WorkerType::CASHIER)
		{
			// cast to get the warnings
			const Cashier* cashier = dynamic_cast<const Cashier*>(allWorkers[i]);
			if (cashier)
			{
				int totalPoints = cashier->getTotalWarningPoints();
				if (totalPoints > pointsThreshold)
				{
					foundAny = true;
					std::cout << "ID: " << cashier->getId()
						<< " | Name: " << cashier->getFirstName() << " " << cashier->getLastName()
						<< " | Points: " << totalPoints << std::endl;
				}
			}
		}
	}

	if (!foundAny)
	{
		std::cout << "No cashiers found matching the criteria." << std::endl;
	}
	std::cout << "-----------------------------------------------" << std::endl;
}

bool Manager::warnCashier(Worker* cashierToWarn, WarningLevel level, const char* description) const
{
	// not a cashier
	if (!cashierToWarn || cashierToWarn->getWorkerType() != WorkerType::CASHIER)
	{
		return false;
	}

	Cashier* target = dynamic_cast<Cashier*>(cashierToWarn);
	if (target)
	{
		Warning newWarning(this->getFirstName(), description, level);
		target->addWarning(newWarning);
		return true;
	}

	return false;
}

bool Manager::promoteCashier(Worker** allWorkers, size_t workersCount, size_t cashierIdToPromote, const char* specialCode) const
{
	if (!validateSpecialCode(specialCode))
	{
		std::cout << "addfajeoifjoawiejfoia";
		return false;
	}

	int targetIndex = findWorkerIndexById(cashierIdToPromote, allWorkers, workersCount);

	if (targetIndex == -1)
	{
		return false;
	}

	// not a cashier or not approved
	if (allWorkers[targetIndex]->getWorkerType() != WorkerType::CASHIER || !allWorkers[targetIndex]->isApproved()) 
	{
		return false; // Грешка: Може да се повишават само одобрени касиери
	}

	Worker* cashierToPromote = allWorkers[targetIndex];

	// copy data
	size_t savedId = cashierToPromote->getId();
	size_t savedAge = cashierToPromote->getAge();
	char* savedFirstName = nullptr;
	StrOpr::strCopy(savedFirstName, cashierToPromote->getFirstName());
	char* savedLastName = nullptr;
	StrOpr::strCopy(savedLastName, cashierToPromote->getLastName());
	char* savedPassword = nullptr;
	StrOpr::strCopy(savedPassword, cashierToPromote->getPassword());
	char savedPhone[Constants::PHONENUMBER_SIZE + 1];
	for (int i = 0; i < Constants::PHONENUMBER_SIZE; ++i) {
		savedPhone[i] = cashierToPromote->getPhoneNumber()[i];
	}
	savedPhone[Constants::PHONENUMBER_SIZE] = '\0';

	// delete old cashier
	delete cashierToPromote;
	allWorkers[targetIndex] = nullptr;

	// create new manager
	Manager* newManager = new Manager(savedId, savedFirstName, savedLastName, savedPhone, savedAge, savedPassword);
	allWorkers[targetIndex] = newManager;

	// generate code and save to file //TODO:
	char* newSpecialCode = newManager->generateAndSaveSpecialCode();
	char* newManagerIdStr = StrOpr::size_tToChar(savedId);
	char* newManagerFileName = StrOpr::concatChar(newManagerIdStr, FileName::SPECIAL_CODE);
	std::cout << "New special code for promoted manager " << savedFirstName << " is: " << newSpecialCode
		<< " (saved to " << newManagerFileName << ")" << std::endl;

	// delete
	delete[] savedFirstName;
	delete[] savedLastName;
	delete[] savedPassword;
	delete[] newSpecialCode;
	delete[] newManagerIdStr;
	delete[] newManagerFileName;

	return true;
}

bool Manager::fireCashier(Worker** allWorkers, size_t& workersCount, size_t cashierIdToFire, const char* specialCode) const
{
	// check special code
	if(!validateSpecialCode(specialCode)) 
	{
		return false;
	}

	// find cashier
	int targetIndex = findWorkerIndexById(cashierIdToFire, allWorkers, workersCount);

	// no such cashier
	if (targetIndex == -1) {
		return false;
	}

	// not a cashier
	if (allWorkers[targetIndex]->getWorkerType() != WorkerType::CASHIER) {
		return false;
	}

	// delete cashier
	delete allWorkers[targetIndex];
	for (size_t i = targetIndex; i < workersCount - 1; i++)
	{
		allWorkers[i] = allWorkers[i + 1];
	}
	workersCount--;
	allWorkers[workersCount] = nullptr;

	return true;
}

bool Manager::addCategory(ProductCategory**& categories, size_t& count, size_t& capacity, const char* name, const char* description) const
{
	// check if already exists
	for (size_t i = 0; i < count; i++)
	{
		if (categories[i] && StrOpr::equals(categories[i]->getName(), name))
		{
			return false; // existing name
		}
	}

	// check capacity
	if (count >= capacity) 
	{
		size_t newCapacity = (capacity == 0) ? Constants::DEFAULT_CAPACITY : capacity * 2;
		COMMON::resize(categories, count, newCapacity);
		capacity = newCapacity;
	}

	// find next id
	size_t newId = 1;
	if (count > 0) 
	{
		size_t maxId = 0;
		for (size_t i = 0; i < count; i++)
		{
			if (categories[i] && categories[i]->getId() > maxId) 
			{
				maxId = categories[i]->getId();
			}
		}
		newId = maxId + 1;
	}

	// create product category
	categories[count] = new ProductCategory(newId, name, description);
	count++;

	return true;
}

bool Manager::deleteCategory(ProductCategory**& categories, size_t& categoryCount, size_t categoryIdToDelete, Product** allProducts, size_t productsCount) const
{
	// check if category is empty
	for (size_t i = 0; i < productsCount; i++)
	{
		if (allProducts[i] && allProducts[i]->getCategory()->getId() == categoryIdToDelete)
		{
			return false; // not empty, cannot delete
		}
	}

	// find category to delete
	int targetIndex = -1;
	for (size_t i = 0; i < categoryCount; i++) 
	{
		if (categories[i] && categories[i]->getId() == categoryIdToDelete) 
		{
			targetIndex = i;
			break;
		}
	}

	// no such category
	if (targetIndex == -1)
	{
		return false;
	}

	// delete category
	delete categories[targetIndex];
	for (size_t i = targetIndex; i < categoryCount - 1; i++)
	{
		categories[i] = categories[i + 1];
	}
	categoryCount--;
	categories[categoryCount] = nullptr;

	return true;
}

bool Manager::addProduct(Product**& products, size_t& productsCount, size_t& productsCapacity, ProductCategory** allCategories, size_t categoryCount, ProductType type) const
{
	const int BUFFER_SIZE = 1024;
	char name[BUFFER_SIZE];
	char categoryName[BUFFER_SIZE];
	double price;
	double quantity;

	// 1. get product name and category
	std::cout << "\tEnter product name: ";
	std::cin.getline(name, BUFFER_SIZE);
	if (name[0] == '\0') {
		std::cin.getline(name, BUFFER_SIZE);
	}
	std::cout << "Enter product category name: ";
	std::cin.getline(categoryName, BUFFER_SIZE);

	// 2. validate category name
	const ProductCategory* foundCategory = ProductCategory::findByName(categoryName, allCategories, categoryCount);

	if (!foundCategory) 
	{
		std::cout << "Error: Category '" << categoryName << "' not found. Please add the category first." << std::endl;
		return false; // no category found, cannot add product
	}

	// 3. get price and quantity
	const char* pricePrompt = (type == ProductType::ByUnit) ? "Enter price per unit: " : "Enter price per kg: ";
	std::cout << pricePrompt;
	std::cin >> price;
	const char* qtyPrompt = (type == ProductType::ByUnit) ? "Enter quantity (units): " : "Enter quantity (kg): ";
	std::cout << qtyPrompt;
	std::cin >> quantity;
	std::cin.ignore();// clear the newline character from the input buffer

	// 4. check if product already exists
	for (size_t i = 0; i < productsCount; i++) 
	{
		if (products[i] && StrOpr::equals(products[i]->getName(), name)) 
		{
			std::cout << "Error: A product with the name '" << name << "' already exists." << std::endl;
			return false; // product with the same name already exists
		}
	}

	// 5. check capacity
	if (productsCount >= productsCapacity)
	{
		size_t newCapacity = (productsCapacity == 0) ? Constants::DEFAULT_CAPACITY : productsCapacity * 2;
		COMMON::resize(products, productsCount, newCapacity);
		productsCapacity = newCapacity;
	}

	// 6. find next product id
	size_t newId = 1;
	if (productsCount > 0) 
	{
		size_t maxId = 0;
		for (size_t i = 0; i < productsCount; i++)
		{
			if (products[i] && products[i]->getId() > maxId)
			{
				maxId = products[i]->getId();
			}
		}
		newId = maxId + 1;
	}

	// 7. create new product
	if (type == ProductType::ByUnit) 
	{
		products[productsCount] = new ProductByUnit(newId, name, price, foundCategory, static_cast<size_t>(quantity));
	}
	else
	{
		products[productsCount] = new ProductByWeight(newId, name, price, foundCategory, quantity);
	}
	productsCount++;

	return true;
}

bool Manager::deleteProduct(Product**& products, size_t& productsCount, size_t productIdToDelete) const
{
	// find product to delete
	int targetIndex = -1;
	for (size_t i = 0; i < productsCount; i++)
	{
		if (products[i] && products[i]->getId() == productIdToDelete)
		{
			targetIndex = i;
			break;
		}
	}

	//no such product
	if (targetIndex == -1)
	{
		return false;
	}

	// delete product
	delete products[targetIndex];
	for (size_t i = targetIndex; i < productsCount - 1; i++)
	{
		products[i] = products[i + 1];
	}
	productsCount--;
	products[productsCount] = nullptr;

	return true;
}

bool Manager::loadProducts(const char* filename, Product**& products, size_t& productsCount, size_t& productsCapacity, ProductCategory**& categories, size_t& categoryCount, size_t& categoryCapacity) const
{
	// check if file exists
	std::ifstream file(filename);
	if (!file.is_open()) 
	{
		std::cout << "Error: Could not open file '" << filename << "'." << std::endl;
		return false;
	}

	char line[1024];
	int lineNum = 0;
	int productsUpdated = 0;
	int productsAdded = 0;
	int categoriesAdded = 0;

	while (file.getline(line, 1024))
	{
		lineNum++;
		// format <NAME>:<CATEGORY>:<TYPE>:<PRICE>:<QUANTITY>
		char* lineRunner = line;
		char* name = StrOpr::strtok(lineRunner, ':');
		char* categoryName = StrOpr::strtok(lineRunner, ':');
		char* typeStr = StrOpr::strtok(lineRunner, ':');
		char* priceStr = StrOpr::strtok(lineRunner, ':');
		char* quantityStr = StrOpr::strtok(lineRunner, ':');

		// invalid format check
		if (!name || !categoryName || !typeStr || !priceStr || !quantityStr)
		{
			std::cout << "Warning: Invalid format on line " << lineNum << ". Skipping." << std::endl;
			continue;
		}

		double price = StrOpr::to_double(priceStr);
		double quantity = StrOpr::to_double(quantityStr);

		// check if product exists
		Product* existingProduct = nullptr;
		for (size_t i = 0; i < productsCount; i++)
		{
			if (StrOpr::equals(products[i]->getName(), name)) 
			{
				existingProduct = products[i];
				break;
			}
		}

		// existing product found
		if (existingProduct) 
		{
			existingProduct->increaseQuantity(quantity);
			productsUpdated++;
		}
		else // new product
		{
			ProductCategory* productCategory = nullptr;

			// check if category exists
			for (size_t i = 0; i < categoryCount; i++) 
			{
				if (StrOpr::equals(categories[i]->getName(), categoryName)) 
				{
					productCategory = categories[i];
					break;
				}
			}

			// new category
			if (!productCategory) 
			{
				// check capacity
				if (categoryCount >= categoryCapacity) 
				{
					size_t newCap = (categoryCapacity == 0) ? Constants::DEFAULT_CAPACITY : categoryCapacity * 2;
					COMMON::resize(categories, categoryCount, newCap);
					categoryCapacity = newCap;
				}
				size_t newCatId = (categoryCount > 0) ? (categories[categoryCount - 1]->getId() + 1) : 1; // find next id
				categories[categoryCount] = new ProductCategory(newCatId, categoryName, "No description");
				productCategory = categories[categoryCount];
				categoryCount++;
				categoriesAdded++;
			}

			// check capacity for products
			if (productsCount >= productsCapacity) 
			{
				size_t newCap = (productsCapacity == 0) ? Constants::DEFAULT_CAPACITY : productsCapacity * 2;
				COMMON::resize(products, productsCount, newCap);
				productsCapacity = newCap;
			}

			// create new product
			size_t newProdId = (productsCount > 0) ? (products[productsCount - 1]->getId() + 1) : 1; // find next id
			ProductType type = StrOpr::equals(typeStr, "unit") ? ProductType::ByUnit : ProductType::ByWeight;

			if (type == ProductType::ByUnit) 
			{
				products[productsCount] = new ProductByUnit(newProdId, name, price, productCategory, static_cast<size_t>(quantity));
			}
			else 
			{
				products[productsCount] = new ProductByWeight(newProdId, name, price, productCategory, quantity);
			}
			productsCount++;
			productsAdded++;
		}
	}

	std::cout << "Product loading complete from file '" << filename << "'." << std::endl;
	if (productsUpdated > 0) std::cout << " - Stock updated for " << productsUpdated << " existing products." << std::endl;
	if (productsAdded > 0) std::cout << " - Added " << productsAdded << " new products." << std::endl;
	if (categoriesAdded > 0) std::cout << " - Auto-created " << categoriesAdded << " new categories." << std::endl;

	file.close();
	return true;
}

bool Manager::loadGiftCards(const char* filename, GiftCard**& giftCards, size_t& giftCardsCount, size_t& giftCardsCapacity, size_t& voucherCounter, ProductCategory** allCategories, size_t categoryCount) const
{
	std::ifstream file(filename);
	if (!file.is_open()) 
	{
		std::cout << "Error: Could not open generation file '" << filename << "'." << std::endl;
		return false;
	}

	char line[1024];
	int lineNum = 0;
	int totalGeneratedCount = 0;

	while (file.getline(line, 1024)) 
	{
		lineNum++;
		char* lineRunner = line;
		char* typeStr = StrOpr::strtok(lineRunner, ':');
		if (!typeStr) continue;

		if (StrOpr::equals(typeStr, "AllProducts"))
		{
			totalGeneratedCount += processAllProductsCard(lineRunner, giftCards, giftCardsCount, giftCardsCapacity, voucherCounter, lineNum);
		}
		else if (StrOpr::equals(typeStr, "SingleCategory"))
		{
			totalGeneratedCount += processSingleCategoryCard(lineRunner, giftCards, giftCardsCount, giftCardsCapacity, voucherCounter, allCategories, categoryCount, lineNum);
		}
		else if (StrOpr::equals(typeStr, "MultipleCategory"))
		{
			totalGeneratedCount += processMultipleCategoryCard(lineRunner, giftCards, giftCardsCount, giftCardsCapacity, voucherCounter, allCategories, categoryCount, lineNum);
		}
	}

	std::cout << "Successfully generated " << totalGeneratedCount << " new gift cards." << std::endl;
	file.close();
	return true;
}

//bool Manager::loadGiftCards(const char* filename, GiftCard**& giftCards, size_t& giftCardsCount, size_t& giftCardsCapacity, size_t& voucherCounter, ProductCategory** allCategories, size_t categoryCount) const
//{
//	std::ifstream file(filename);
//	if (!file.is_open()) 
//	{
//		std::cout << "Error: Could not open generation file '" << filename << "'." << std::endl;
//		return false;
//	}
//
//	char line[1024];
//	int lineNum = 0;
//	int generatedCount = 0;
//
//	while (file.getline(line, 1024)) 
//	{
//		lineNum++;
//		char* lineRunner = line;
//		char* typeStr = StrOpr::strtok(lineRunner, ':');
//		if (!typeStr) continue;
//
//		if (StrOpr::equals(typeStr, "AllProducts")) 
//		{
//			// Format: AllProducts:<PERCENT>:<COUNT>
//			char* percentStr = StrOpr::strtok(lineRunner, ':');
//			char* countStr = StrOpr::strtok(lineRunner, ':');
//			if (!percentStr || !countStr) 
//			{
//				std::cout << "Warning: Invalid format on line " << lineNum << ". Skipping." << std::endl;
//				continue;
//			}
//			double percent = StrOpr::to_double(percentStr);
//			int countToGen = StrOpr::to_size_t(countStr);
//
//			for (int i = 0; i < countToGen; i++) 
//			{
//				voucherCounter++;
//				char* newCode = StrOpr::generateVoucherCode(voucherCounter);
//
//				if (giftCardsCount >= giftCardsCapacity) 
//				{
//					size_t newCap = (giftCardsCapacity == 0) ? 8 : giftCardsCapacity * 2;
//					COMMON::resize(giftCards, giftCardsCount, newCap);
//					giftCardsCapacity = newCap;
//				}
//				giftCards[giftCardsCount++] = new AllProductsGiftCard(newCode, percent);
//				delete[] newCode;
//				generatedCount++;
//			}
//		}
//		else if (StrOpr::equals(typeStr, "SingleCategory")) 
//		{
//			// format: SingleCategory:<CATEGORY_NAME>:<PERCENT>:<COUNT>
//			char* categoryName = StrOpr::strtok(lineRunner, ':');
//			char* percentStr = StrOpr::strtok(lineRunner, ':');
//			char* countStr = StrOpr::strtok(lineRunner, ':');
//			if (!categoryName || !percentStr || !countStr) {
//				std::cout << "Warning: Invalid format on line " << lineNum << ". Skipping." << std::endl;
//				continue;
//			}
//
//			const ProductCategory* foundCategory = ProductCategory::findByName(categoryName, allCategories, categoryCount);
//
//			if (!foundCategory) {
//				std::cout << "Warning: Category '" << categoryName << "' not found on line " << lineNum << ". Skipping." << std::endl;
//				continue;
//			}
//
//			double percent = StrOpr::to_double(percentStr);
//			int countToGen = StrOpr::to_size_t(countStr);
//
//			for (int i = 0; i < countToGen; i++) {
//				voucherCounter++;
//				char* newCode = StrOpr::generateVoucherCode(voucherCounter);
//
//				if (giftCardsCount >= giftCardsCapacity) {
//					size_t newCap = (giftCardsCapacity == 0) ? 8 : giftCardsCapacity * 2;
//					COMMON::resize(giftCards, giftCardsCount, newCap);
//					giftCardsCapacity = newCap;
//				}
//				giftCards[giftCardsCount++] = new SingleCategoryGiftCard(newCode, percent, foundCategory);
//				delete[] newCode;
//				generatedCount++;
//			}
//		}
//		else if (StrOpr::equals(typeStr, "MultipleCategory"))
//		{
//			// Format: MultipleCategory:Cat1,Cat2,Cat3:PERCENT:COUNT
//			char* categoriesStr = StrOpr::strtok(lineRunner, ':');
//			char* percentStr = StrOpr::strtok(lineRunner, ':');
//			char* countStr = StrOpr::strtok(lineRunner, ':');
//			if (!categoriesStr || !percentStr || !countStr) 
//			{
//				std::cout << "Warning: Invalid format for MultipleCategory on line " << lineNum << ". Skipping." << std::endl;
//				continue;
//			}
//
//			// find categories
//			const int MAX_CATS_PER_VOUCHER = 10;
//			const ProductCategory* foundCategories[MAX_CATS_PER_VOUCHER];
//			size_t foundCount = 0;
//
//			char* catName = StrOpr::strtok(categoriesStr, ',');
//			while (catName != nullptr && foundCount < MAX_CATS_PER_VOUCHER) 
//			{
//				ProductCategory* cat = nullptr;
//				for (size_t i = 0; i < categoryCount; i++)
//				{
//					if (StrOpr::equals(allCategories[i]->getName(), catName))
//					{
//						cat = allCategories[i];
//						break;
//					}
//				}
//				if (cat) 
//				{
//					foundCategories[foundCount++] = cat;
//				}
//				else
//				{
//					std::cout << "Warning: Category '" << catName << "' not found on line " << lineNum << ". It will be ignored." << std::endl;
//				}
//				catName = StrOpr::strtok(categoriesStr, ',');
//			}
//
//			if (foundCount == 0) 
//			{
//				std::cout << "Warning: No valid categories found for MultipleCategory voucher on line " << lineNum << ". Skipping." << std::endl;
//				continue;
//			}
//
//			double percent = StrOpr::to_double(percentStr);
//			int countToGen = StrOpr::to_size_t(countStr);
//
//			for (int i = 0; i < countToGen; i++) 
//			{
//				voucherCounter++;
//				char* newCode = StrOpr::generateVoucherCode(voucherCounter);
//
//				if (giftCardsCount >= giftCardsCapacity) 
//				{
//					size_t newCap = (giftCardsCapacity == 0) ? 8 : giftCardsCapacity * 2;
//					COMMON::resize(giftCards, giftCardsCount, newCap);
//					giftCardsCapacity = newCap;
//				}
//				// create new MultipleCategoryGiftCard
//				giftCards[giftCardsCount++] = new MultipleCategoryGiftCard(newCode, percent, foundCategories, foundCount);
//				delete[] newCode;
//				generatedCount++;
//			}
//		}
//	}
//
//	std::cout << "Successfully generated " << generatedCount << " new gift cards." << std::endl;
//	file.close();
//	return true;
//}

char* Manager::generateAndSaveSpecialCode() const
{
	char* specialCode = StrOpr::generateSpecialCode();
	char* idStr = StrOpr::size_tToChar(this->getId());
	char* fileName = StrOpr::concatChar(idStr, FileName::SPECIAL_CODE);

	std::ofstream codeFile(fileName);
	if (codeFile.is_open())
	{
		codeFile << specialCode;
		codeFile.close();
	}

	delete[] idStr;
	delete[] fileName;

	return specialCode;
}

