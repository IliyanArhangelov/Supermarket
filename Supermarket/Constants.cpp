#include "Constants.h"
#include <fstream>
#include <ctime>
#pragma warning(disable:4996)


bool FileOpr::isNumberInFile(int n, std::istream& file)
{
	std::streampos original_pos = file.tellg();
	std::ios_base::iostate original_state = file.rdstate();

	file.clear();
	file.seekg(0, std::ios::beg);

	if (file.fail()) {
		file.clear();

		if (original_pos != std::streampos(-1)) {
			file.seekg(original_pos);
		}

		file.clear(original_state);
		return false;
	}

	int currentNumber = 1;
	bool found = false;

	while (!file.eof()) {
		file >> currentNumber;
		if (currentNumber == n) {
			found = true;
			break;
		}
	}

	file.clear();
	file.seekg(original_pos);
	file.clear(original_state);

	return found;
}

void FileOpr::deleteNumberFromFile(int numberToDelete, const char* filename)
{
	std::ifstream file(filename);
	size_t count = 0;

	int num;
	while (file >> num) {

		count++;
	}

	int* numbers = new int[count];

	file.clear();
	file.seekg(0, std::ios::beg);

	for (size_t i = 0; i < count; i++)
	{
		file >> numbers[i];
	}

	file.close();
	std::ofstream newFile(filename, std::ios::trunc);

	for (size_t i = 0; i < count; i++)
	{
		if (numbers[i] != numberToDelete)
		{
			newFile << numbers[i] << " ";
		}
	}
	delete[] numbers;
}

void FileOpr::printFile(const char* filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Error opening file: " << filename << '\n';
		return;
	}

	char ch;
	while (file.get(ch)) {
		std::cout << ch;
	}
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
	size_t size = strLen(str1);
	if (strLen(str2) != size)
	{
		std::cout << size << std::endl << strLen(str2);
		std::cout << 'n';
		return false;
	}

	for (size_t i = 0; i < size; i++)
	{
		std::cout << 'a';
		if (str1[i] != str2[i])
		{
			std::cout << 'o';
			return false;
		}
	}
	return true;
}
char* StrOpr::concatChar(const char* str1, const char* str2) {
	size_t lenA = strLen(str1);
	size_t lenB = strLen(str2);

	char* result = new char[lenA + lenB + 1];

	for (size_t i = 0; i < lenA; i++) {
		result[i] = str1[i];
	}
	for (size_t i = 0; i < lenB; i++) {
		result[lenA + i] = str2[i];
	}
	result[lenA + lenB] = '\0';
	return result;
}

char* StrOpr::size_tToChar(size_t n)
{
	if (n == 0) {
		char* zeroStr = new char[2];
		zeroStr[0] = '0';
		zeroStr[1] = '\0';
		return zeroStr;
	}

	size_t digits = NumberOpr::digitsCount(n);
	char* result = new char[digits + 1];
	result[digits] = '\0';

	for (size_t i = 0; i < digits; i++) {
		result[digits - i - 1] = '0' + (n % 10);
		n /= 10;
	}

	return result;
}

char* StrOpr::getTime() {
	// The desired format: "HH:MM DD.MM.YYYY"
// This requires 16 characters plus one for the null terminator.
	char* buffer = new char[17];

	// Obtain the current time.
	time_t now = time(NULL);
	struct tm* local = localtime(&now);
	if (!local) {
		buffer[0] = '\0';
		return buffer;
	}

	// Extract the individual components.
	int hour = local->tm_hour;
	int minute = local->tm_min;
	int day = local->tm_mday;
	int month = local->tm_mon + 1; // tm_mon is 0-based.
	int year = local->tm_year + 1900; // tm_year is years since 1900.

	// Manually fill the buffer:
	// Format positions:
	// Index:  0 1   2  3 4   5   6 7   8   9 10 11   12 13 14 15
	//         H H   :  M M   ' ' D D   .  M M  .    Y  Y  Y  Y

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
