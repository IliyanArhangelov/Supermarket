#include "Product.h"
#include "Constants.h"


void Product::free()
{
	delete[] name;
	delete category;
	name = nullptr;
	category = nullptr;
}

void Product::copyFrom(const Product& other)
{
	id = other.id;
	type = other.type;
	price = other.price;

	name = nullptr;
	StrOpr::strCopy(name, other.name);

	if (other.category) {
		category = new ProductCategory(*other.category);
	}
	else {
		category = nullptr;
	}
}

void Product::moveFrom(Product&& other) noexcept
{
	id = other.id;
	type = other.type;
	price = other.price;

	name = other.name;
	category = other.category;

	other.name = nullptr;
	other.category = nullptr;
}

Product::Product(size_t id, ProductType type, const char* name, double price, ProductCategory category)
{
	this->id = id;
	this->type = type;
	this->price = price;

	this->name = nullptr;
	StrOpr::strCopy(this->name, name);

	this->category = new ProductCategory(category);
}

Product::Product(const Product& other)
{
	copyFrom(other);
}

Product::Product(Product&& other) noexcept
{
	moveFrom(std::move(other));
}

Product::~Product()
{
	free();
}

Product& Product::operator=(const Product& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}

Product& Product::operator=(Product&& other) noexcept
{
	if (this != &other)
	{
		free();
		moveFrom(std::move(other));
	}
	return *this;
}