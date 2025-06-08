#include "Worker.h"
#include "Constants.h"
#include "CommonFunctions.hpp"

void Worker::staticCopyFrom(const Worker& other)
{
	COMMON::copyArr<char>(phoneNumber, other.phoneNumber, Constants::PHONENUMBER_SIZE);
	approved = other.approved;
	workerType = other.workerType;
	id = other.id;
	age = other.age;
	transactionCount = other.transactionCount;
	
	warningsCount = other.warningsCount;
	warningsCapacity = other.warningsCapacity;
}
void Worker::dynamicCopyFrom(const Worker& other)
{
	COMMON::copyArr<Warning>(warnings, other.warnings, other.warningsCount);
	StrOpr::strCopy(firstName, other.firstName);
	StrOpr::strCopy(lastName, other.lastName);
	StrOpr::strCopy(password, other.password);
}
void Worker::dynamicMoveFrom(Worker&& other) noexcept
{
	warnings = other.warnings;
	firstName = other.firstName;
	lastName = other.lastName;
	password = other.password;

	other.warnings = nullptr;
	other.firstName = nullptr;
	other.lastName = nullptr;
	other.password = nullptr;
}
void Worker::free()
{
	delete[] firstName;
	delete[] lastName;
	delete[] password;
	delete[] warnings;
	firstName = nullptr;
	lastName = nullptr;
	password = nullptr;
	warnings = nullptr;
}

void Worker::copyFrom(const Worker& other)
{
	staticCopyFrom(other);
	dynamicCopyFrom(other);
}

void Worker::moveFrom(Worker&& other) noexcept
{
	dynamicMoveFrom(std::move(other));
	staticCopyFrom(other);

	//other.free();
}

void Worker::noPermissionMessage() const
{
	std::cout << "NO PERMISSION FOR THIS ACTION" << std::endl;
}

Worker::Worker(WorkerType workerType, bool approved, size_t id, const char* firstName, const char* lastName, const char* phoneNumber, size_t age, const char* password, size_t transactionCount, size_t warningsCount, size_t warningsCapacity, const Warning* warnings)
{
	COMMON::copyArr<char>(this->phoneNumber, phoneNumber, Constants::PHONENUMBER_SIZE);
	COMMON::copyArr<Warning>(this->warnings, warnings, warningsCount);

	StrOpr::strCopy(this->firstName, firstName);
	StrOpr::strCopy(this->lastName, lastName);
	StrOpr::strCopy(this->password, password);

	this->approved = approved;
	this->workerType = workerType;
	this->id = id;
	this->age = age;
	this->transactionCount = transactionCount;
	this->warningsCount = warningsCount;
	this->warningsCapacity = warningsCapacity;
}

Worker::Worker(const Worker& other)
{
	copyFrom(other);
}

Worker::Worker(Worker&& other) noexcept
{
	moveFrom(std::move(other));
}

Worker::~Worker()
{
	free();
}

Worker& Worker::operator=(const Worker& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}

Worker& Worker::operator=(Worker&& other) noexcept
{
	if (this != &other)
	{
		free();
		moveFrom(std::move(other));
	}
	return *this;
}

void Worker::sell() {
	noPermissionMessage();
}


void Worker::listPending() const {
	noPermissionMessage();
}

void Worker::approve(Worker* cashier, const char* specialCode) const {
	noPermissionMessage();
}

void Worker::decline(Worker* cashier, const char* specialCode) const {
	noPermissionMessage();
}

void Worker::listWarnedCashier(Worker** workers, size_t points) const {
	noPermissionMessage();
}

void Worker::warnCashier(Worker* cashier, size_t points) const {
	noPermissionMessage();
}

void Worker::promoteCashier(Worker* cashier, const char* specialCode) const {
	noPermissionMessage();
}

void Worker::fireCashier(Worker** workers, size_t cashierId, const char* specialCode) const {
	noPermissionMessage();
}

void Worker::addCategory(size_t categoryName, const char* categoryDescription) const {
	noPermissionMessage();
}

void Worker::deleteCategory(size_t categoryId) const {
	noPermissionMessage();
}

void Worker::addProduct(ProductType productType) const {
	noPermissionMessage();
}

void Worker::deleteProduct(size_t productId) const {
	noPermissionMessage();
}

void Worker::loadProducts(const char* filename) const {
	noPermissionMessage();
}

void Worker::loadGiftcards(const char* filename) const {
	noPermissionMessage();
}
