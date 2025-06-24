#include "ProductCategory.h"
#include "Constants.h"
#include <iostream>
#include <fstream>

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

const ProductCategory* ProductCategory::findById(size_t id, ProductCategory** allCategories, size_t count)
{
	for (size_t i = 0; i < count; i++) 
	{
		if (allCategories[i] && allCategories[i]->getId() == id)
		{
			return allCategories[i];
		}
	}
	return nullptr;
}

const ProductCategory* ProductCategory::findByName(const char* name, ProductCategory** allCategories, size_t count)
{
	for (size_t i = 0; i < count; i++) 
	{
		if (allCategories[i] && StrOpr::equals(allCategories[i]->getName(), name))
		{
			return allCategories[i];
		}
	}
	return nullptr;
}

void ProductCategory::serialize(std::ostream& os) const
{
	os << id << ' ';
	FileOpr::writeString(os, name);
	FileOpr::writeString(os, description);
}
size_t ProductCategory::getId() const
{ 
	return id;
}

const char* ProductCategory::getName() const
{ 
	return name; 
}

const char* ProductCategory::getDescription() const
{ 
	return description;
}