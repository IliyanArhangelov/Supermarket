#pragma once
#include <fstream>
#include <iostream>

enum class WorkerType
{
	MANAGER,
	CASHIER
};

enum class WarningLevel {
	LOW = 100,
	MEDIUM = 200,
	HIGH = 300
};

enum class ProductType
{
	ByUnit,
	ByWeight
};

enum class GiftCardType {
	SingleCategory,
	MultipleCategory,
	AllProducts
};

namespace Constants
{
	const size_t SPECIAL_CODE_LENGTH = 7;
	const size_t PHONENUMBER_SIZE = 10;
	const size_t DEFAULT_CAPACITY = 10;
}

namespace FileName
{

	const char DATA_DIR[] = "data/";
	const char RECEIPTS_DIR[] = "data/receipts/";
	const char SECURE_DIR[] = "data/secure/";

	const char WORKERS[] = "data/Workers.txt";
	const char PRODUCTS[] = "data/Products.txt";
	const char PRODUCT_CATEGORY[] = "data/ProductCategory.txt";
	const char FEED[] = "data/feed.txt";
	const char LOG[] = "data/log.txt";
	const char TRANSACTIONS[] = "data/Transactions.txt";
	const char GIFT_CARDS[] = "data/GiftCards.txt";

	const char RECEIPT_PREFIX[] = "data/receipts/receipt_";
	const char SPECIAL_CODE_SUFFIX[] = "_special_code.txt";
}

namespace FileOpr
{
	void printFile(const char* filename);
	void writeString(std::ostream& os, const char* str);
	char* readString(std::istream& is);
}

namespace NumberOpr
{
	size_t digitsCount(int n);
}

namespace StrOpr
{

	size_t strLen(const char* str);
	void strCopy(char*& dest, const char* src);
	bool equals(const char* str1, const char* str2);
	char* concatChar(const char* str1, const char* str2);

	char* size_tToChar(size_t n);
	char* getTime();

	char* strtok(char*& str, char delimiter);
	size_t to_size_t(const char* str);
	double to_double(const char* str);

	char* generateSpecialCode();
	char* generateVoucherCode(size_t counter);
}




