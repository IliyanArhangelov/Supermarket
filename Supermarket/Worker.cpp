#include "Worker.h"
#include "Constants.h"
#include "CommonFunctions.hpp"
#include "Transaction.h"

void Worker::staticCopyFrom(const Worker& other)
{
	COMMON::copyArr<char>(phoneNumber, other.phoneNumber, Constants::PHONENUMBER_SIZE);
	approved = other.approved;
	workerType = other.workerType;
	id = other.id;
	age = other.age;
}
void Worker::dynamicCopyFrom(const Worker& other)
{
	StrOpr::strCopy(firstName, other.firstName);
	StrOpr::strCopy(lastName, other.lastName);
	StrOpr::strCopy(password, other.password);
}
void Worker::dynamicMoveFrom(Worker&& other) noexcept
{
	firstName = other.firstName;
	lastName = other.lastName;
	password = other.password;
	other.firstName = nullptr;
	other.lastName = nullptr;
	other.password = nullptr;
}
void Worker::free()
{
	delete[] firstName;
	delete[] lastName;
	delete[] password;
	firstName = nullptr;
	lastName = nullptr;
	password = nullptr;
}

void Worker::copyFrom(const Worker& other)
{
	staticCopyFrom(other);
	dynamicCopyFrom(other);
}

void Worker::moveFrom(Worker&& other) noexcept
{	
	staticCopyFrom(other);
	dynamicMoveFrom(std::move(other));
}

void Worker::noPermissionMessage() const
{
	std::cout << "NO PERMISSION FOR THIS ACTION" << std::endl;
}

Worker::Worker(WorkerType workerType, bool approved, size_t id, const char* firstName, const char* lastName, const char* phoneNumber, size_t age, const char* password)
{
	this->approved = approved;
	this->workerType = workerType;
	this->id = id;
	this->age = age;

	COMMON::copyArr(this->phoneNumber, phoneNumber, Constants::PHONENUMBER_SIZE);

	this->firstName = nullptr;
	this->lastName = nullptr;
	this->password = nullptr;
	StrOpr::strCopy(this->firstName, firstName);
	StrOpr::strCopy(this->lastName, lastName);
	StrOpr::strCopy(this->password, password);
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

void Worker::serialize(std::ostream& os) const
{
	os << static_cast<int>(workerType) << ' ';
	os << id << ' ';
	FileOpr::writeString(os, firstName);
	FileOpr::writeString(os, lastName);
	os.write(phoneNumber, Constants::PHONENUMBER_SIZE);
	os << ' ' << age << ' ';
	FileOpr::writeString(os, password);
	os << approved << ' ';
}

bool Worker::sell(Product** allProducts, size_t productsCount, Transaction**& transactions, size_t& transactionsCount, size_t& transactionsCapacity, GiftCard** allGiftCards, size_t giftCardsCount)
{
	noPermissionMessage();
	return false;
}

size_t Worker::getId() const 
{
	return id;

}
const char* Worker::getFirstName() const 
{
	return firstName; 
}

const char* Worker::getLastName() const 
{ 
	return lastName;
}

const char* Worker::getPassword() const
{ 
	return password;
}

const char* Worker::getPhoneNumber() const 
{ 
	return phoneNumber; 
}

size_t Worker::getAge() const 
{
	return age;
}

bool Worker::isApproved() const 
{
	return approved;
}

void Worker::setApproved(bool isApproved)
{
	this->approved = isApproved;
}

WorkerType Worker::getWorkerType() const 
{ 
	return workerType; 
}

void Worker::listPending(Worker** allWorkers, size_t count) const
{
	noPermissionMessage();
}

bool Worker::approve(Worker* cashierToApprove, const char* specialCode) const 
{
	noPermissionMessage();
	return false;
}

bool Worker::decline(Worker** allWorkers, size_t& count, size_t cashierIdToDecline, const char* specialCode) const
{
	noPermissionMessage();
	return false;
}

void Worker::listWarnedCashiers(Worker** allWorkers, size_t workersCount, int pointsThreshold) const
{
	noPermissionMessage();
}

bool Worker::warnCashier(Worker* cashierToWarn, WarningLevel level, const char* description) const 
{
	noPermissionMessage();
	return false;
}

bool Worker::promoteCashier(Worker** allWorkers, size_t workersCount, size_t cashierIdToPromote, const char* specialCode) const 
{
	noPermissionMessage();
	return false;
}

bool Worker::fireCashier(Worker** allWorkers, size_t& workersCount, size_t cashierIdToFire, const char* specialCode) const
{
	noPermissionMessage();
	return false;
}

bool Worker::addCategory(ProductCategory**& categories, size_t& count, size_t& capacity, const char* name, const char* description) const 
{
	noPermissionMessage();
	return false;
}

bool Worker::deleteCategory(ProductCategory**& categories, size_t& categoryCount, size_t categoryIdToDelete, Product** allProducts, size_t productsCount) const
{
	noPermissionMessage();
	return false;
}

bool Worker::addProduct(Product**& products, size_t& productsCount, size_t& productsCapacity, ProductCategory** allCategories, size_t categoryCount, ProductType type) const 
{
	noPermissionMessage();
	return false;
}

bool Worker::deleteProduct(Product**& products, size_t& productsCount, size_t productIdToDelete) const
{
	noPermissionMessage();
	return false;
}

bool Worker::loadProducts(const char* filename, Product**& products, size_t& productsCount, size_t& productsCapacity, ProductCategory**& categories, size_t& categoryCount, size_t& categoryCapacity) const
{
	noPermissionMessage();
	return false;
}

bool Worker::loadGiftCards(const char* filename, GiftCard**& giftCards, size_t& giftCardsCount, size_t& giftCardsCapacity, size_t& voucherCounter, ProductCategory** allCategories, size_t categoryCount) const
{
	noPermissionMessage();
	return false;
}
