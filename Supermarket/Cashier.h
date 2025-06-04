#pragma once
#include "Worker.h"
#include "Constants.h"

class Cashier : public Worker
{
public:
	Cashier() = delete;
	Cashier(size_t id, const char* firstName, const char* lastName, const char* phoneNumber, size_t age,
		const char* password, size_t transactionCount, size_t warningsCount, size_t warningsCapacity, const Warning* warnings);
	Cashier(const Cashier& other);
	Cashier(Cashier&&) noexcept;
	~Cashier();

	Cashier& operator=(const Cashier& other);
	Cashier&& operator=(Cashier&& other);

	void sell() override;
};

