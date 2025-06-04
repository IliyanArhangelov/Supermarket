#include "System.h"

void System::free()
{
	delete currentUser;
	delete[] workers;
	delete[] productCategory;
	delete[] products;

	currentUser = nullptr;
	workers = nullptr;
	productCategory = nullptr;
	products = nullptr;
}
