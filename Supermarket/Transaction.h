#pragma once
#include "Constants.h"
#include <iostream>

struct Transaction
{
	size_t id;
	size_t cashierId;
	double totalSum;
	char* dateTime;
	char* receiptFileName;

	Transaction(size_t _id, size_t _cashierId, double _totalSum, const char* _dateTime, const char* _receiptFileName)
	{
		id = _id;
		cashierId = _cashierId;
		totalSum = _totalSum;
		dateTime = nullptr;
		receiptFileName = nullptr;
		StrOpr::strCopy(dateTime, _dateTime);
		StrOpr::strCopy(receiptFileName, _receiptFileName);
	}

	~Transaction()
	{
		delete[] dateTime;
		delete[] receiptFileName;
	}

	Transaction(const Transaction& other) = delete;
	Transaction& operator=(const Transaction& other) = delete;

	void serialize(std::ostream& os) const
	{
		os << id << ' ' << cashierId << ' ' << totalSum << ' ';
		FileOpr::writeString(os, dateTime);
		FileOpr::writeString(os, receiptFileName);
	}
};
