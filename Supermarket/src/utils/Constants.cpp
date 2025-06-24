#include "Constants.h"
#include <fstream>
#include <ctime>
#pragma warning(disable:4996)

void FileOpr::printFile(const char* filename) 
{
	std::ifstream file(filename);
	if (!file.is_open()) 
	{
		std::cerr << "Error opening file: " << filename << '\n';
		return;
	}

	char ch;
	while (file.get(ch)) 
	{
		std::cout << ch;
	}
}

void FileOpr::writeString(std::ostream& os, const char* str)
{
	if (!str)
	{
		os << 0 << ' ';
		return;
	}
	size_t len = StrOpr::strLen(str);
	os << len << ' ';
	if (len > 0) 
	{ 
		os.write(str, len);
	}
	os << ' ';
}

char* FileOpr::readString(std::istream& is)
{
	size_t len;
	is >> len;

	// get the empty space ' ' or '\n'
	is.get();

	if (len == 0)
	{
		return new char[1]{'\0'};
	}

	char* buffer = new char[len + 1];
	is.read(buffer, len);
	buffer[len] = '\0';

	return buffer;
}

size_t NumberOpr::digitsCount(int n)
{
	if (n == 0)
	{
		return 1;
	}

	size_t digits = 0;
	while (n > 0) {
		++digits;
		n /= 10;
	}
	return digits;
}

size_t StrOpr::strLen(const char* str)
{
	size_t size = 0;
	while (*str)
	{
		size++;
		str++;
	}
	return size;
}
void StrOpr::strCopy(char*& dest, const char* src)
{
	delete[] dest;
	size_t len = strLen(src);

	dest = new char[len + 1];
	for (size_t i = 0; i <= len; i++)
	{
		dest[i] = src[i];
	}

}
bool StrOpr::equals(const char* str1, const char* str2)
{
	if (!str1 || !str2) {
		return str1 == str2;
	}
	while (*str1 != '\0' && *str2 != '\0')
	{
		if (*str1 != *str2) {
			return false;
		}
		str1++;
		str2++;
	}
	return *str1 == *str2;
}
char* StrOpr::concatChar(const char* str1, const char* str2) {
	size_t lenA = strLen(str1);
	size_t lenB = strLen(str2);

	char* result = new char[lenA + lenB + 1];

	for (size_t i = 0; i < lenA; i++)
	{
		result[i] = str1[i];
	}
	for (size_t i = 0; i < lenB; i++)
	{
		result[lenA + i] = str2[i];
	}
	result[lenA + lenB] = '\0';
	return result;
}

char* StrOpr::size_tToChar(size_t n)
{
	if (n == 0)
	{
		char* zeroStr = new char[2];
		zeroStr[0] = '0';
		zeroStr[1] = '\0';
		return zeroStr;
	}

	size_t digits = NumberOpr::digitsCount(n);
	char* result = new char[digits + 1];
	result[digits] = '\0';

	for (size_t i = 0; i < digits; i++)
	{
		result[digits - i - 1] = '0' + (n % 10);
		n /= 10;
	}

	return result;
}

char* StrOpr::getTime()
{
	char* buffer = new char[17];
	time_t now = time(NULL);
	struct tm* local = localtime(&now);
	if (!local) {
		buffer[0] = '\0';
		return buffer;
	}

	int hour = local->tm_hour;
	int minute = local->tm_min;
	int day = local->tm_mday;
	int month = local->tm_mon + 1;
	int year = local->tm_year + 1900;

	// Hour: two digits
	buffer[0] = '0' + (hour / 10);
	buffer[1] = '0' + (hour % 10);

	// Colon separator.
	buffer[2] = ':';

	// Minute: two digits
	buffer[3] = '0' + (minute / 10);
	buffer[4] = '0' + (minute % 10);

	// Space separator.
	buffer[5] = ' ';

	// Day: two digits
	buffer[6] = '0' + (day / 10);
	buffer[7] = '0' + (day % 10);

	// Dot separator.
	buffer[8] = '.';

	// Month: two digits
	buffer[9] = '0' + (month / 10);
	buffer[10] = '0' + (month % 10);

	// Dot separator.
	buffer[11] = '.';

	// Year: four digits
	buffer[12] = '0' + ((year / 1000) % 10);
	buffer[13] = '0' + ((year / 100) % 10);
	buffer[14] = '0' + ((year / 10) % 10);
	buffer[15] = '0' + (year % 10);

	// Null terminator.
	buffer[16] = '\0';

	return buffer;
}

char* StrOpr::strtok(char*& str, char delimiter)
{
	if (!str || *str == '\0') {
		return nullptr;
	}
	char* start = str;
	while (*str != '\0' && *str != delimiter) {
		str++;
	}
	if (*str != '\0') {
		*str = '\0';
		str++;
	}
	return start;
}

size_t StrOpr::to_size_t(const char* str)
{
	if (!str) { return 0; }
	size_t result = 0;
	while (*str >= '0' && *str <= '9') {
		result = result * 10 + (*str - '0');
		str++;
	}
	return result;
}

double StrOpr::to_double(const char* str)
{
	if (!str) { return 0.0; }

	double result = 0.0;
	double sign = 1.0;

	// check negative sign
	if (*str == '-') {
		sign = -1.0;
		str++;
	}

	// 1. find numbers before the decimal point
	while (*str >= '0' && *str <= '9') {
		result = result * 10.0 + (*str - '0');
		str++;
	}

	// 2. find decimal point
	if (*str == '.' || *str == ',') {
		str++;

		double fraction = 0.0;
		double divisor = 1.0;

		// 3. find numbers after the decimal point
		while (*str >= '0' && *str <= '9') {
			fraction = fraction * 10.0 + (*str - '0');
			divisor *= 10.0;
			str++;
		}

		// 4. add the fraction to the result
		result += fraction / divisor;
	}

	return result * sign;
}

char* StrOpr::generateSpecialCode()
{
	// Format: <0-9><A-Z><A-Z><0-9><0-9><0-9><a-z> 
	char* code = new char[Constants::SPECIAL_CODE_LENGTH + 1];

	static bool seeded = false;
	if (!seeded) {
		srand(time(0));
		seeded = true;
	}

	// <0-9>
	code[0] = '0' + (rand() % 10);
	// <A-Z>
	code[1] = 'A' + (rand() % 26);
	// <A-Z>
	code[2] = 'A' + (rand() % 26);
	// <0-9>
	code[3] = '0' + (rand() % 10);
	// <0-9>
	code[4] = '0' + (rand() % 10);
	// <0-9>
	code[5] = '0' + (rand() % 10);
	// <a-z>
	code[6] = 'a' + (rand() % 26);
	// end of string
	code[7] = '\0';

	return code;
}

char* StrOpr::generateVoucherCode(size_t counter)
{
	char* counterStr = StrOpr::size_tToChar(counter);
	size_t counterLen = StrOpr::strLen(counterStr);

	char* code = new char[4 + counterLen + 1];

	// init generator
	static bool seeded = false;
	if (!seeded) {
		srand(time(0));
		seeded = true;
	}

	// generating parts
	code[0] = '0' + (rand() % 10);		 // <0-9>
	code[1] = 'A' + (rand() % 26);		 // <A-Z>

	// Копираме counter-а в средата
	for (size_t i = 0; i < counterLen; i++) {
		code[2 + i] = counterStr[i];
	}

	code[2 + counterLen] = 'A' + (rand() % 26); // <A-Z>
	code[3 + counterLen] = '0' + (rand() % 10); // <0-9>
	code[4 + counterLen] = '\0';				

	delete[] counterStr; 
	return code;
}
