#pragma once
#include "models/worker/Worker.h"
#include "models/product/Product.h"
#include "models/product/ProductCategory.h"
#include "models/transaction/Transaction.h"
#include "models/giftcard/GiftCard.h"

class System
{
	Worker* currentUser;
	Worker** workers;
	size_t workersCount;
	size_t workersCapacity;

	ProductCategory** productCategory;
	size_t productCategoryCount;
	size_t productCategoryCapacity;

	Product** products;
	size_t productsCount;
	size_t productsCapacity;

	Transaction** transactions;
	size_t transactionsCount;
	size_t transactionsCapacity;

	GiftCard** giftCards;
	size_t giftCardsCount;
	size_t giftCardsCapacity;
	size_t voucherCounter; 

	void loadWorkers();
	void loadProducts();
	void loadProductCategory();
	void loadGiftCardsFromFile(); 
	void loadTransactions();

	void saveTransactions() const;
	void saveGiftCardsToFile() const; 
	void saveWorkersToFile() const;
	void saveProductCategoriesToFile() const;
	void saveProductsToFile() const;

	void free();
	void emptyInit();

	void createDefaultManager();
	void printHelp() const;
	void listTransactions() const;

	Worker* findWorkerById(size_t id) const;
	const ProductCategory* findCategoryById(size_t id) const;
	const ProductCategory* findCategoryByName(const char* name) const;

public:
	System();
	System(const System& other) = delete;
	System& operator=(const System& other) = delete;
	~System();

	void action(const char* userInput);
	void login(size_t id, const char* password);
	void registerUser(WorkerType type, const char* firstName, const char* lastName,
	const char* phone, size_t age, const char* password);
	void listUserData() const;
	void listWorkers() const;
	void listProducts() const;
	void listProducts(size_t categoryId) const;
	void listProductCategories() const;
	void logToFeed(const char* message);
	void listFeed() const;
	void listGiftCards() const;
	void leave();
	void logout();


	void writeToFiles();
};

