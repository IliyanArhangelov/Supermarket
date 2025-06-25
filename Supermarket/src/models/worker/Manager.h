#pragma once
#include "models/worker/Worker.h"

class Manager : public Worker
{
	bool validateSpecialCode(const char* specialCode) const;
	

	int processAllProductsCard(char*& lineRunner, GiftCard**& giftCards, size_t& giftCardsCount, size_t& giftCardsCapacity, size_t& voucherCounter, int lineNum) const;
	int processSingleCategoryCard(char*& lineRunner, GiftCard**& giftCards, size_t& giftCardsCount, size_t& giftCardsCapacity, size_t& voucherCounter, ProductCategory** allCategories, size_t categoryCount, int lineNum) const;
	int processMultipleCategoryCard(char*& lineRunner, GiftCard**& giftCards, size_t& giftCardsCount, size_t& giftCardsCapacity, size_t& voucherCounter, ProductCategory** allCategories, size_t categoryCount, int lineNum) const;
public:
	Manager() = delete;
	Manager(size_t id, const char* firstName, const char* lastName, const char* phoneNumber, size_t age, const char* password);
	Manager(const Manager& other);
	Manager(Manager&& other) noexcept;
	~Manager();

	Manager& operator=(const Manager& other);
	Manager& operator=(Manager&& other) noexcept;

	void listPending(Worker** allWorkers, size_t count) const override;
	bool approve(Worker* cashierToApprove, const char* specialCode) const override;
	bool decline(Worker** allWorkers, size_t& count, size_t cashierIdToDecline, const char* specialCode) const override;
	void listWarnedCashiers(Worker** allWorkers, size_t workersCount, int pointsThreshold) const override;
	bool warnCashier(Worker* cashierToWarn, WarningLevel level, const char* description) const override;
	bool promoteCashier(Worker** allWorkers, size_t workersCount, size_t cashierIdToPromote, const char* specialCode) const override;
	bool fireCashier(Worker** allWorkers, size_t& workersCount, size_t cashierIdToFire, const char* specialCode) const override;
	bool addCategory(ProductCategory**& categories, size_t& count, size_t& capacity, const char* name, const char* description) const override;
	bool deleteCategory(ProductCategory**& categories, size_t& categoryCount, size_t categoryIdToDelete, Product** allProducts, size_t productsCount) const override;
	bool addProduct(Product**& products, size_t& productsCount, size_t& productsCapacity, ProductCategory** allCategories, size_t categoryCount, ProductType type) const override;
	bool deleteProduct(Product**& products, size_t& productsCount, size_t productIdToDelete) const override;
	bool loadProducts(const char* filename, Product**& products, size_t& productsCount, size_t& productsCapacity, ProductCategory**& categories, size_t& categoryCount, size_t& categoryCapacity) const override;
	bool loadGiftCards(const char* filename, GiftCard**& giftCards, size_t& giftCardsCount, size_t& giftCardsCapacity, size_t& voucherCounter, ProductCategory** allCategories, size_t categoryCount) const override;
	static int findWorkerIndexById(size_t workerId, Worker** allWorkers, size_t workersCount);

	char* generateAndSaveSpecialCode() const;
};