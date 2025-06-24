#include "Product.h"
#include "Constants.h"

void Product::free()
{
	delete[] name;
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
	category = other.category;
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

Product::Product(size_t id, ProductType type, const char* name, double price, const ProductCategory* category)
{
    this->id = id;
    this->type = type;
    this->price = price;
    this->name = nullptr;
    StrOpr::strCopy(this->name, name);
    this->category = const_cast<ProductCategory*>(category);
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

void Product::serialize(std::ostream& os) const
{
	os << static_cast<int>(type) << ' ';
	os << id << ' ';
	FileOpr::writeString(os, name);
	os << price << ' ';
	os << category->getId() << ' ';
}

size_t Product::getId() const
{ 
	return id;
}
const char* Product::getName() const 
{
	return name; 
}
double Product::getPrice() const 
{
	return price; 
}
ProductType Product::getProductType() const 
{
	return type; 
}
const ProductCategory* Product::getCategory() const 
{
	return category; 
}