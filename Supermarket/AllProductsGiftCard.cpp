#include "AllProductsGiftCard.h"

AllProductsGiftCard::AllProductsGiftCard(const char* _code, double _percentage)
	: GiftCard(_code, _percentage, GiftCardType::AllProducts)
{
}

double AllProductsGiftCard::apply(const CartItem* cart, size_t cartCount) const
{
	double totalDiscount = 0;
	for (size_t i = 0; i < cartCount; i++)
	{
		totalDiscount += (cart[i].pricePerItem * cart[i].quantity);
	}
	return totalDiscount * (percentage / 100.0);
}

GiftCard* AllProductsGiftCard::clone() const 
{
	return new AllProductsGiftCard(*this);
}

void AllProductsGiftCard::serialize(std::ostream& os) const
{
	GiftCard::serialize(os);
}