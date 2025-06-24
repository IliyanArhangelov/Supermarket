#include "SingleCategoryGiftCard.h"

SingleCategoryGiftCard::SingleCategoryGiftCard(const char* _code, double _percentage, const ProductCategory* _category)
	: GiftCard(_code, _percentage, GiftCardType::SingleCategory), category(_category)
{
}

const ProductCategory* SingleCategoryGiftCard::getCategory() const 
{
	return category;
}

double SingleCategoryGiftCard::apply(const CartItem* cart, size_t cartCount) const
{
	if (!category) return 0;

	double totalDiscountBase = 0;
	for (size_t i = 0; i < cartCount; i++)
	{
		if (cart[i].categoryId == category->getId()) {
			totalDiscountBase += (cart[i].pricePerItem * cart[i].quantity);
		}
	}
	return totalDiscountBase * (percentage / 100.0);
}

GiftCard* SingleCategoryGiftCard::clone() const
{
	return new SingleCategoryGiftCard(*this);
}

void SingleCategoryGiftCard::serialize(std::ostream& os) const 
{
	GiftCard::serialize(os);
	os << category->getId(); 
}
