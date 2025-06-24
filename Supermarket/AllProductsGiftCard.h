#pragma once
#include "GiftCard.h"

class AllProductsGiftCard : public GiftCard
{
public:
	AllProductsGiftCard(const char* _code, double _percentage);
	double apply(const CartItem* cart, size_t cartCount) const override;
	GiftCard* clone() const override;
	void serialize(std::ostream& os) const override;
};