#include "Manager.h"

Manager::Manager(size_t id, const char* firstName, const char* lastName, const char* phoneNumber, size_t age,
	const char* password, size_t transactionCount, size_t warningsCount, size_t warningsCapacity, const Warning* warnings)
	: Worker(WorkerType::MANAGER, true, id, firstName, lastName, phoneNumber, age, password, transactionCount, warningsCount, warningsCapacity, warnings)
{
}

Manager::Manager(const Manager& other)
	: Worker(other)
{
}

Manager::Manager(Manager&& other) noexcept
	: Worker(std::move(other))
{
}

Manager::~Manager()
{
}

Manager& Manager::operator=(const Manager& other)
{
	if (this != &other)
	{
		Worker::operator=(other);
	}
	return *this;
}

Manager& Manager::operator=(Manager&& other) noexcept
{
	if (this != &other)
	{
		Worker::operator=(std::move(other));
	}
	return *this;
}
