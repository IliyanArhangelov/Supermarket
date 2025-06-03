#pragma once
#include "Worker.h"
class System
{
	Worker* currentUser;
	Worker* workers;

	void free();

public:
	System();
	System(const char* workersFilename, const char* productsFilename);
	System& operator=(const System& other) = delete;

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
};

