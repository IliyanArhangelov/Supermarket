#pragma once
#include "Constants.h"
#include "Cashier.h"
class Worker
{
	bool approved;
	WorkerType workerType;
	size_t id;
	char* firstName;
	//size_t firstNameSize;
	char* lastName;
	//ize_t lastNameSize;
	char phoneNumber[Constants::PHONENUMBER_SIZE];
	size_t age;
	char* password;

	size_t transactionCount;
	Warning* warnings;
	size_t warningsCount;
	size_t warningsCapacity;

	void staticCopyFrom(const Worker& other);
	void dynamicCopyFrom(const Worker& other);
	void dynamicMoveFrom(Worker&& other) noexcept;
	void free();
	void copyFrom(const Worker& other);
	void moveFrom(Worker&& other) noexcept;
	void noPermissionMessage() const;

public:
	//for every worker
	Worker() = delete;
	Worker(WorkerType workerType, bool approved, size_t id, const char* firstName, const char* lastName, const char* phoneNumber, size_t age, 
		const char* password, size_t transactionCount, size_t warningsCount, size_t warningsCapacity, const Warning* warnings);
	Worker(const Worker& other);
	Worker(Worker&& other) noexcept;
	virtual ~Worker();

	Worker& operator=(const Worker& other);
	Worker& operator=(Worker&& other) noexcept;

	//for Cashier
	virtual void sell();

	//for Manager
	virtual void listPending() const;
	virtual void approve(Worker* cashier, const char* specialCode) const;
	virtual void decline(Worker* cashier, const char* specialCode) const;
	virtual void listWarnedCashier(Worker** workers, size_t points) const;
	virtual void warnCashier(Worker* cashier, size_t points) const;
	virtual void promoteCashier(Worker* cashier, const char* specialCode) const;
	virtual void fireCashier(Worker** workers, size_t cashierId, const char* specialCode) const;
	virtual void addCategory(size_t categoryName, const char* categoryDescription) const;
	virtual void deleteCategory(size_t categoryId) const;
	virtual void addProduct(ProductType productType) const;
	virtual void deleteProduct(size_t productId) const;
	virtual void loadProducts(const char* filename) const;
	virtual void loadGiftcards(const char* filename) const;
};

