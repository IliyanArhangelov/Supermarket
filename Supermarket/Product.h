#pragma once
#include "Constants.h"
#include "ProductCategory.h"

class Product
{
private:
	size_t id;
	ProductType type;
	char* name;
	double price;
	ProductCategory* category;

	void free();
	void copyFrom(const Product& other);
	void moveFrom(Product&& other) noexcept;

public:
	Product(size_t id, ProductType type, const char* name, double price, const ProductCategory* category);
	Product() = delete;
	Product(const Product& other);
	Product(Product&& other) noexcept;
	Product& operator=(const Product& other);
	Product& operator=(Product&& other) noexcept;
	virtual ~Product(); 

	virtual void serialize(std::ostream& os) const;

	// Getters
	size_t getId() const;
	const char* getName() const;
	double getPrice() const;
	ProductType getProductType() const;
	const ProductCategory* getCategory() const;


	//pure virtual
	virtual double getQuantity() const = 0;
	virtual void increaseQuantity(double amount) = 0;
	virtual void decreaseQuantity(double amount) = 0;
	virtual Product* clone() const = 0;
};