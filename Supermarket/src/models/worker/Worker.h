#pragma once
#include "models/product/ProductCategory.h"
#include "utils/Constants.h"
#include "models/product/Product.h"
#include "models/transaction/Transaction.h"
#include "models/giftcard/GiftCard.h"

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
		const char* password);
	Worker(const Worker& other);
	Worker(Worker&& other) noexcept;
	virtual ~Worker();

	Worker& operator=(const Worker& other);
	Worker& operator=(Worker&& other) noexcept;

	size_t getId() const;
	const char* getFirstName() const;

	const char* getLastName() const;
	const char* getPassword() const;
	const char* getPhoneNumber() const;
	size_t getAge() const;
	bool isApproved() const;
	void setApproved(bool isApproved);
	WorkerType getWorkerType() const;

	virtual void serialize(std::ostream& os) const;

	//for Cashier
	virtual bool sell(Product** allProducts, size_t productsCount, Transaction**& transactions, size_t& transactionsCount, size_t& transactionsCapacity, GiftCard** allGiftCards, size_t giftCardsCount);

	//for Manager
	virtual void listPending(Worker** allWorkers, size_t count) const;
	virtual bool approve(Worker* cashierToApprove, const char* specialCode) const;
	virtual bool decline(Worker** allWorkers, size_t& count, size_t cashierIdToDecline, const char* specialCode) const;
	virtual void listWarnedCashiers(Worker** allWorkers, size_t workersCount, int pointsThreshold) const;
	virtual bool warnCashier(Worker* cashierToWarn, WarningLevel level, const char* description) const;
	virtual bool promoteCashier(Worker** allWorkers, size_t workersCount, size_t cashierIdToPromote, const char* specialCode) const;
	virtual bool fireCashier(Worker** allWorkers, size_t& workersCount, size_t cashierIdToFire, const char* specialCode) const;
	virtual bool addCategory(ProductCategory**& categories, size_t& count, size_t& capacity, const char* name, const char* description) const;
	virtual bool deleteCategory(ProductCategory**& categories, size_t& categoryCount, size_t categoryIdToDelete, Product** allProducts, size_t productsCount) const;	virtual bool addProduct(Product**& products, size_t& productsCount, size_t& productsCapacity, ProductCategory** allCategories, size_t categoryCount, ProductType type) const;
	virtual bool deleteProduct(Product**& products, size_t& productsCount, size_t productIdToDelete) const;
	virtual bool loadProducts(const char* filename, Product**& products, size_t& productsCount, size_t& productsCapacity, ProductCategory**& categories, size_t& categoryCount, size_t& categoryCapacity) const;
	virtual bool loadGiftCards(const char* filename, GiftCard**& giftCards, size_t& giftCardsCount, size_t& giftCardsCapacity, size_t& voucherCounter, ProductCategory** allCategories, size_t categoryCount) const;
};

