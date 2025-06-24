#pragma once
#include "Constants.h"
#include "Worker.h"
#include "Warning.h"
#include "Transaction.h"

class Cashier : public Worker
{
private:
	size_t transactionCount;
	Warning** warnings;
	size_t warningsCount;
	size_t warningsCapacity;

	void free();
	void copyFrom(const Cashier& other);
	void moveFrom(Cashier&& other) noexcept;

public:
	Cashier() = delete;
	Cashier(bool approved, size_t id, const char* firstName, const char* lastName, const char* phoneNumber, size_t age,
		const char* password, size_t transactionCount, const Warning* const* initialWarnings, size_t warningsCount);

	Cashier(const Cashier& other);
	Cashier(Cashier&& other) noexcept;
	~Cashier();
	Cashier& operator=(const Cashier& other);
	Cashier& operator=(Cashier&& other) noexcept;

	void serialize(std::ostream& os) const override;
	void addWarning(const Warning& newWarning); 

	size_t getTotalWarningPoints() const;
	size_t getTransactionCount() const;
	size_t getWarningsCount() const;
	const Warning* getWarning(size_t index) const;

	bool sell(Product** allProducts, size_t productsCount, Transaction**& transactions, size_t& transactionsCount, size_t& transactionsCapacity, GiftCard** allGiftCards, size_t giftCardsCount) override;
};
