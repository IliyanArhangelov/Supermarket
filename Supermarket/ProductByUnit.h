#pragma once
#include "Product.h"

class ProductByUnit : public Product
{
private:
	size_t units; 

public:
	ProductByUnit(size_t id, const char* name, double price, const ProductCategory* category, size_t initialUnits);
	void increaseQuantity(double amount) override;
	void serialize(std::ostream& os) const override;
	double getQuantity() const override;
	void decreaseQuantity(double amount) override;
	Product* clone() const override;
};
