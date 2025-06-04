#pragma once
#include "Constants.h"
#include "Worker.h"

class Manager: public Worker
{
public:
	Manager() = delete;
	Manager(size_t id, const char* firstName, const char* lastName, const char* phoneNumber, size_t age,
		const char* password, size_t transactionCount, size_t warningsCount, size_t warningsCapacity, const Warning* warnings);
	Manager(const Manager& other);
	Manager(Manager&&) noexcept;
	~Manager();

	Manager& operator=(const Manager& other);
	Manager&& operator=(Manager&& other);

	void listPending() const override;
	void approve(size_t cashierId, const char* specialCode) const override;
	void decline(size_t cashierId, const char* specialCode) const override;
	void listWarnedCashier(size_t points) const override;
	void warnCashier(size_t cashierId, size_t points) const override;
	void promoteCashier(size_t cashierId, const char* specialCode) const override;
	void fireCashier(size_t cashierId, const char* specialCode) const override;
	void addCategory(size_t categoryName, const char* categoryDescription) const override;
	void deleteCategory(size_t categoryId) const override;
	void addProduct(ProductType productType) const override;
	void deleteProduct(size_t productId) const override;
	void loadProducts(const char* filename) const override;
	void loadGiftcards(const char* filename) const override;
};

