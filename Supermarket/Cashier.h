#pragma once
#include "Worker.h"
#include "Constants.h"

class Cashier : public Worker
{
	size_t transactionCount;
	Warning* warnings;
	void free();
	void copyFrom(const Cashier& other);
	void moveFrom(Cashier&& other);

};

