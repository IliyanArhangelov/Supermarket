#include "ProductByUnit.h"

ProductByUnit::ProductByUnit(size_t id, const char* name, double price, const ProductCategory* category, size_t initialUnits)
	: Product(id, ProductType::ByUnit, name, price, category)
{
	this->units = initialUnits;
}

void ProductByUnit::increaseQuantity(double amount)
{
	if (amount > 0)
	{
		units += static_cast<size_t>(amount);
	}
}

void ProductByUnit::serialize(std::ostream& os) const
{
	Product::serialize(os);
	os << units << ' ';
}

double ProductByUnit::getQuantity() const
{
	return units;
}

void ProductByUnit::decreaseQuantity(double amount)
{
	size_t amountAsUnits = static_cast<size_t>(amount);
	if (amountAsUnits <= units)
	{
		units -= amountAsUnits;
	}
}

Product* ProductByUnit::clone() const
{
	return new ProductByUnit(*this);
}