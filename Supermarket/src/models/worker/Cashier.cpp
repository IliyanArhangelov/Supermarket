#include "models/worker/Cashier.h"
#include "utils/CommonFunctions.hpp"
#include "models/transaction/CartItem.h"
#include "models/giftcard/AllProductsGiftCard.h"
#include "models/giftcard/SingleCategoryGiftCard.h"


void Cashier::free()
{
	for (size_t i = 0; i < warningsCount; i++)
	{
		delete warnings[i];
	}
	delete[] warnings;
	warnings = nullptr; 
	warningsCount = 0;
	warningsCapacity = 0;
}


void Cashier::copyFrom(const Cashier& other)
{
	transactionCount = other.transactionCount;
	warningsCount = other.warningsCount;
	warningsCapacity = other.warningsCapacity;

	warnings = new Warning * [warningsCapacity];
	for (size_t i = 0; i < warningsCount; i++)
	{
		warnings[i] = new Warning(*other.warnings[i]);
	}
}

void Cashier::moveFrom(Cashier&& other) noexcept
{
	transactionCount = other.transactionCount;
	warningsCount = other.warningsCount;
	warningsCapacity = other.warningsCapacity;
	warnings = other.warnings;

	other.transactionCount = 0;
	other.warningsCount = 0;
	other.warningsCapacity = 0;
	other.warnings = nullptr;
}

Cashier::Cashier(bool approved, size_t id, const char* firstName, const char* lastName, const char* phoneNumber, size_t age,
	const char* password, size_t transactionCount, const Warning* const* initialWarnings, size_t warningsCount)
	: Worker(WorkerType::CASHIER, approved, id, firstName, lastName, phoneNumber, age, password) 
{

	this->transactionCount = transactionCount;
	this->warningsCount = 0;
	this->warningsCapacity = (warningsCount > 0) ? warningsCount + Constants::DEFAULT_CAPACITY : Constants::DEFAULT_CAPACITY;
	this->warnings = new Warning * [this->warningsCapacity];

	for (size_t i = 0; i < warningsCount; i++)
	{
		if (initialWarnings && initialWarnings[i])
		{
			addWarning(*initialWarnings[i]);
		}
	}
}

Cashier::~Cashier()
{
	free();
}

Cashier::Cashier(const Cashier& other) : Worker(other)
{
	copyFrom(other);
}

Cashier::Cashier(Cashier&& other) noexcept : Worker(std::move(other))
{
	moveFrom(std::move(other));
}

Cashier& Cashier::operator=(const Cashier& other)
{
	if (this != &other)
	{
		free();
		Worker::operator=(other);
		copyFrom(other);
	}
	return *this;
}

Cashier& Cashier::operator=(Cashier&& other) noexcept
{
	if (this != &other)
	{
		free();
		Worker::operator=(std::move(other));
		moveFrom(std::move(other));
	}
	return *this;
}

void Cashier::serialize(std::ostream& os) const
{
	Worker::serialize(os);
	os << transactionCount << ' ' << warningsCount << ' ';
	for (size_t i = 0; i < warningsCount; i++)
	{
		warnings[i]->serialize(os);
	}
}

void Cashier::addWarning(const Warning& newWarning)
{
	if (warningsCount >= warningsCapacity)
	{
		size_t newCapacity = (warningsCapacity == 0) ? Constants::DEFAULT_CAPACITY : warningsCapacity * 2;
		COMMON::resize(warnings, warningsCount, newCapacity);
		warningsCapacity = newCapacity;
	}
	warnings[warningsCount++] = new Warning(newWarning);
}

size_t Cashier::getTotalWarningPoints() const
{
	int totalPoints = 0;
	for (size_t i = 0; i < warningsCount; i++)
	{
		if (warnings[i])
		{
			totalPoints += static_cast<int>(warnings[i]->getLevel());
		}
	}
	return totalPoints;
}

size_t Cashier::getTransactionCount() const 
{
	return transactionCount; 
}

size_t Cashier::getWarningsCount() const
{ 
	return warningsCount;
}

const Warning* Cashier::getWarning(size_t index) const
{
	if (index < warningsCount)
	{
		return warnings[index];
	}
	return nullptr;
}

bool Cashier::sell(Product** allProducts, size_t productsCount, Transaction**& transactions, size_t& transactionsCount, size_t& transactionsCapacity, GiftCard** allGiftCards, size_t giftCardsCount)
{
	if (!this->isApproved())
	{
		std::cout << "Error: Your account is not approved by a manager. You cannot perform sales." << std::endl;
		return false;
	}

	const int MAX_CART_ITEMS = 100;
	CartItem cart[MAX_CART_ITEMS];
	size_t cartCount = 0;
	double totalPrice = 0;

	size_t newTransactionId = (transactionsCount > 0) ? transactions[transactionsCount - 1]->id + 1 : 1;

	while (true)
	{
		std::cout << "--------- \nProducts:" << std::endl;
		for (size_t i = 0; i < productsCount; i++)
		{
			if (allProducts[i]) 
			{
				std::cout << allProducts[i]->getId() << ". " << allProducts[i]->getName() << " : "
					<< allProducts[i]->getPrice() << (allProducts[i]->getProductType() == ProductType::ByUnit ? "" : "/kg")
					<< " : " << allProducts[i]->getQuantity() << std::endl;
			}
		}
		std::cout << "Transaction ID: " << newTransactionId << std::endl;
		std::cout << "Price: " << totalPrice << std::endl;
		std::cout << "Enter product ID to sell. Enter END to end the transaction:" << std::endl;
		std::cout << "> ";

		char input[32];
		std::cin >> input;

		if (StrOpr::equals(input, "END")) 
		{
			break;
		}

		size_t productId = StrOpr::to_size_t(input);
		Product* targetProduct = nullptr;
		for (size_t i = 0; i < productsCount; i++) 
		{
			if (allProducts[i] && allProducts[i]->getId() == productId) 
			{
				targetProduct = allProducts[i];
				break;
			}
		}

		if (!targetProduct)
		{
			std::cout << "Invalid product ID." << std::endl;
			continue;
		}

		std::cout << "Enter quantity:" << std::endl << "> ";
		double quantity;
		std::cin >> quantity;

		if (quantity <= 0 || quantity > targetProduct->getQuantity())
		{
			std::cout << "Invalid or insufficient quantity." << std::endl;
			continue;
		}

		targetProduct->decreaseQuantity(quantity);
		totalPrice += targetProduct->getPrice() * quantity;

		cart[cartCount].productId = productId;
		cart[cartCount].categoryId = targetProduct->getCategory()->getId();
		cart[cartCount].quantity = quantity;
		cart[cartCount].pricePerItem = targetProduct->getPrice();
		cartCount++;
	}

	if (cartCount == 0)
	{
		std::cout << "Transaction cancelled." << std::endl;
		return false;
	}

	double discount = 0;
	char appliedVoucherCode[256] = "N/A";

	std::cout << "Add voucher: (Y/N)? ";
	char voucherChoice;
	std::cin >> voucherChoice;
	std::cin.ignore();

	if (voucherChoice == 'Y' || voucherChoice == 'y')
	{
		std::cout << "Enter voucher code: ";
		char inputCode[256];
		std::cin.getline(inputCode, 256);

		GiftCard* foundCard = nullptr;
		for (size_t i = 0; i < giftCardsCount; i++) 
		{
			if (allGiftCards[i] && StrOpr::equals(allGiftCards[i]->getCode(), inputCode))
			{
				foundCard = allGiftCards[i];
				break;
			}
		}

		if (foundCard) 
		{
			discount = foundCard->apply(cart, cartCount);
			strcpy_s(appliedVoucherCode, 256, foundCard->getCode());
			std::cout << "Voucher '" << appliedVoucherCode << "' applied successfully! Discount: " << discount << " lv." << std::endl;
		}
		else 
		{
			std::cout << "Invalid voucher code. Continuing without discount." << std::endl;
		}
	}

	double finalPrice = totalPrice - discount;
	if (finalPrice < 0) 
	{
		finalPrice = 0;
	}

	char* transactionIdStr = StrOpr::size_tToChar(newTransactionId);
	char* receiptFileNameWithPrefix = StrOpr::concatChar(FileName::RECEIPT_PREFIX, transactionIdStr);
	char* finalReceiptName = StrOpr::concatChar(receiptFileNameWithPrefix, ".txt");
	std::ofstream receiptFile(finalReceiptName);
	receiptFile << "RECEIPT" << std::endl;
	receiptFile << "TRANSACTION_ID:" << newTransactionId << std::endl;
	receiptFile << "CASHIER_ID:" << this->getId() << std::endl;

	char* timeStr = StrOpr::getTime();
	receiptFile << timeStr << std::endl;
	receiptFile << "------------------------------" << std::endl;

	for (size_t i = 0; i < cartCount; i++) 
	{
		Product* p = nullptr;
		for (size_t j = 0; j < productsCount; j++)
		{
			if (allProducts[j] && allProducts[j]->getId() == cart[i].productId)
			{
				p = allProducts[j];
				break;
			}
		}
		if (p) 
		{
			receiptFile << p->getName() << std::endl;
			receiptFile << cart[i].quantity << "*" << p->getPrice() << " - " << (cart[i].quantity * p->getPrice()) << std::endl;
			receiptFile << "###" << std::endl;
		}
	}
	receiptFile << "------------------------------" << std::endl;
	receiptFile << "SUBTOTAL:" << totalPrice << std::endl;
	receiptFile << "DISCOUNT (Voucher: " << appliedVoucherCode << "):-" << discount << std::endl;
	receiptFile << "TOTAL:" << finalPrice << std::endl;
	receiptFile.close();

	std::cout << "Transaction complete!" << std::endl;
	std::cout << "Receipt saved as: " << finalReceiptName << std::endl;

	std::cout << "Total: " << finalPrice << " lv." << std::endl;

	if (transactionsCount >= transactionsCapacity)
	{
		size_t newCap = (transactionsCapacity == 0) ? Constants::DEFAULT_CAPACITY : transactionsCapacity * 2;
		COMMON::resize(transactions, transactionsCount, newCap);
		transactionsCapacity = newCap;
	}

	transactions[transactionsCount++] = new Transaction(newTransactionId, this->getId(), finalPrice, timeStr, finalReceiptName);

	this->transactionCount++;

	if (this->transactionCount % 3 == 0 && this->warningsCount > 0)
	{
		delete warnings[0];
		for (size_t i = 0; i < warningsCount - 1; i++) {
			warnings[i] = warnings[i + 1];
		}
		warningsCount--;
		std::cout << "Good job! One warning has been removed." << std::endl;
	}

	delete[] transactionIdStr;
	delete[] receiptFileNameWithPrefix;
	delete[] finalReceiptName;
	delete[] timeStr;

	return true;
}