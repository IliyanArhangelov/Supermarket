#include "Cashier.h"
#include "Constants.h"

Cashier::Cashier(bool approved, size_t id, const char* firstName, const char* lastName, const char* phoneNumber, size_t age,
	const char* password, size_t transactionCount, size_t warningsCount, size_t warningsCapacity, const Warning* warnings)
	: Worker(WorkerType::CASHIER, approved, id, firstName, lastName, phoneNumber, age, password, transactionCount, warningsCount, warningsCapacity, warnings)
{
}

Cashier::Cashier(const Cashier& other)
	: Worker(other)
{
}

Cashier::Cashier(Cashier&& other) noexcept
	: Worker(std::move(other))
{
}

Cashier::~Cashier()
{
}

Cashier& Cashier::operator=(const Cashier& other)
{
	if (this != &other)
	{
		Worker::operator=(other);
	}
	return *this;
}

Cashier& Cashier::operator=(Cashier&& other) noexcept
{
	if (this != &other)
	{
		Worker::operator=(std::move(other));
	}
	return *this;
}