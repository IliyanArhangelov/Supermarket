#pragma once
#include "GiftCard.h"
#include "ProductCategory.h"

class MultipleCategoryGiftCard : public GiftCard
{
    const ProductCategory** categories; 
    size_t categoriesCount;
    size_t categoriesCapacity;

    void free();
    void copyFrom(const MultipleCategoryGiftCard& other);

public:
    MultipleCategoryGiftCard(const char* _code, double _percentage, const ProductCategory** _categories, size_t _count);

    ~MultipleCategoryGiftCard();
    MultipleCategoryGiftCard(const MultipleCategoryGiftCard& other);
    MultipleCategoryGiftCard& operator=(const MultipleCategoryGiftCard& other);
    MultipleCategoryGiftCard(MultipleCategoryGiftCard&& other) = delete;
    MultipleCategoryGiftCard& operator=(MultipleCategoryGiftCard&& other) = delete;

    double apply(const CartItem* cart, size_t cartCount) const override;
    GiftCard* clone() const override;
    void serialize(std::ostream& os) const override;
};
