#pragma once
//#include <fstream>
#include <iostream>

enum class Warning {
	LOW = 100,
	MEDIUM = 200,
	HIGH = 300
};

enum class ProductType
{
	ByUnit,
	ByType
};

namespace FileName
{
	const char USERS[] = "Users.txt";
	const char ASSIGNMENTS[] = "Assignments.txt";
	const char USERLIST[] = "UserList.txt";
	const char MAILBOX[] = "Mailbox.txt";
	const char GRADES[] = "Grades.txt";
	const char COURSE[] = "Course.txt";
}

namespace FileOpr
{
	bool isNumberInFile(int n, std::istream& file);
	void deleteNumberFromFile(int numberToDelete, const char* filename);
	void printFile(const char* filename);
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
}


