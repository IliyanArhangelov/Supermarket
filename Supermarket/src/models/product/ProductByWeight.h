#pragma once
#include "models/product/Product.h"

class ProductByWeight : public Product
{
private:
	double kgs;

public:
	ProductByWeight(size_t id, const char* name, double price, const ProductCategory* category, double initialKgs);

	void serialize(std::ostream& os) const override;
	double getQuantity() const override;
	void increaseQuantity(double amount) override;
	void decreaseQuantity(double amount) override;
	Product* clone() const override;
};