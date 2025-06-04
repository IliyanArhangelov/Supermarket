#pragma once
#include "Constants.h"
#include "ProductCategory.h"

class Product
{
	size_t id;
	ProductType type;
	const char* name;
	double price;
	ProductCategory category;

	void free();
	void copyFrom(const Product& other);
	void moveFrom(Product&& other) noexcept;
public:
	Product() = delete;
	Product(size_t id, ProductType type, const char* name, double price, ProductCategory category);
	Product(const Product& other);
	Product(Product&& other) noexcept;
	~Product();
	
	Product& operator=(const Product& ohter);
	Product& operator=(Product&& other) noexcept;
};

