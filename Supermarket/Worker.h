#pragma once
#include "Constants.h"

class Worker
{
	size_t id;
	char* firstName;
	char* lastName;
	char* phoneNumber;
	size_t age;
	char* password;

	size_t transactionCount;
	Warning* warnings;
	size_t warningsCount;
	size_t warningsCapacity;



	void free();
	void copyFrom(const Worker& other);
	void moveFrom(Worker&& other) noexcept;
	void noPermissionMessage();

public:
	//for every worker
	Worker() = delete;
	Worker(const char* firstName, const char* lastName, const char* phoneNumber, size_t age, const char* password);
	Worker(const Worker& other);
	Worker(Worker&& other) noexcept;
	virtual ~Worker();

	Worker& operator=(const Worker& other);
	Worker& operator=(Worker&& other) noexcept;

	//for Cashier
	virtual void sell();

	//for Manager
	virtual void listPending() const;
	virtual void approve(size_t cashierId, const char* specialCode) const;
	virtual void decline(size_t cashierId, const char* specialCode) const;
	virtual void listWarnedCashier(size_t points) const;
	virtual void warnCashier(size_t cashierId, size_t points) const;
	virtual void promoteCashier(size_t cashierId, const char* specialCode) const;
	virtual void fireCashier(size_t cashierId, const char* specialCode) const;
	virtual void addCategory(size_t categoryName, const char* categoryDescription) const;
	virtual void deleteCategory(size_t categoryId) const;
	virtual void addProduct(ProductType productType) const;
	virtual void deleteProduct(size_t productId) const;
};

