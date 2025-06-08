#pragma once
#include "Worker.h"
#include "Product.h"
#include "ProductCategory.h"
class System
{
	Worker* currentUser;
	Worker* workers;
	size_t workersCount;
	size_t workersCapacity;

	ProductCategory* productCategory;
	size_t productCategoryCount;
	size_t productCategoryCapacity;

	Product* products;
	size_t productsCount;
	size_t productsCapacity;

	void free();
	void loadWorkers(const char* filename);
	void loadProducts(const char* filename);
	void loadProductCategory(const char* filename);
public:
	System();
	System(const char* workersFilename, const char* productsFilename);
	System& operator=(const System& other) = delete;
	~System();

	void action(const char* userInput);
	void login(size_t id, const char* password);
	void listUserData() const;
	void listWorkers() const;
	void listProducts() const;
	void listProducts(size_t categoryId) const;
	void listFeed() const;
	void listTransactions() const;
	void leave();
	void logout();


	void writeToFiles();
};

