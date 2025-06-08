#include "ProductCategory.h"
#include "Constants.h"

void ProductCategory::free()
{
	delete[] name;
	delete[] description;
	name = nullptr;
	description = nullptr;
}

void ProductCategory::copyFrom(const ProductCategory& other)
{
	id = other.id;
	name = nullptr;
	description = nullptr;
	StrOpr::strCopy(name, other.name);
	StrOpr::strCopy(description, other.description);
}

void ProductCategory::moveFrom(ProductCategory&& other) noexcept
{
	id = other.id;
	name = other.name;
	description = other.description;
	other.name = nullptr;
	other.description = nullptr;
}

ProductCategory::ProductCategory(size_t id, const char* name, const char* description)
{
	this->id = id;
	this->name = nullptr;
	this->description = nullptr;
	StrOpr::strCopy(this->name, name);
	StrOpr::strCopy(this->description, description);
}

ProductCategory::ProductCategory(const ProductCategory& other)
{
	copyFrom(other);
}

ProductCategory::ProductCategory(ProductCategory&& other) noexcept
{
	moveFrom(std::move(other));
}

ProductCategory::~ProductCategory()
{
	free();
}

ProductCategory& ProductCategory::operator=(const ProductCategory& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}

ProductCategory& ProductCategory::operator=(ProductCategory&& other) noexcept
{
	if (this != &other)
	{
		free();
		moveFrom(std::move(other));
	}
	return *this;
}