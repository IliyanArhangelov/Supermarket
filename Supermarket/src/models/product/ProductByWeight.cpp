#include "ProductByWeight.h"

ProductByWeight::ProductByWeight(size_t id, const char* name, double price, const ProductCategory* category, double initialKgs)
	: Product(id, ProductType::ByWeight, name, price, category) 
{
	this->kgs = initialKgs;
}

void ProductByWeight::serialize(std::ostream& os) const
{
	Product::serialize(os);
	os << kgs << ' ';
}

double ProductByWeight::getQuantity() const
{
	return kgs;
}

void ProductByWeight::increaseQuantity(double amount)
{
	if (amount > 0)
	{
		kgs += amount;
	}
}

void ProductByWeight::decreaseQuantity(double amount)
{
	if (amount <= kgs)
	{
		kgs -= amount;
	}
}

Product* ProductByWeight::clone() const
{
	return new ProductByWeight(*this);
}