#pragma once
#include "GiftCard.h"
#include "ProductCategory.h"

class SingleCategoryGiftCard : public GiftCard
{
	const ProductCategory* category;

public:
	const ProductCategory* getCategory() const;
	SingleCategoryGiftCard(const char* _code, double _percentage, const ProductCategory* _category);
	double apply(const CartItem* cart, size_t cartCount) const override;
	GiftCard* clone() const override;
	void serialize(std::ostream& os) const override;
};
