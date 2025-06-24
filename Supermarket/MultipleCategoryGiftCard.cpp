#include "MultipleCategoryGiftCard.h"

void MultipleCategoryGiftCard::free()
{
    delete[] categories;
    categories = nullptr;
    categoriesCount = 0;
    categoriesCapacity = 0;
}

void MultipleCategoryGiftCard::copyFrom(const MultipleCategoryGiftCard& other) 
{
    categoriesCount = other.categoriesCount;
    categoriesCapacity = other.categoriesCapacity;
    categories = new const ProductCategory * [categoriesCapacity];
    for (size_t i = 0; i < categoriesCount; i++)
    {
        categories[i] = other.categories[i];
    }
}


MultipleCategoryGiftCard::MultipleCategoryGiftCard(const char* _code, double _percentage, const ProductCategory** _categories, size_t _count)
    : GiftCard(_code, _percentage, GiftCardType::MultipleCategory)
{
    categoriesCount = _count;
    categoriesCapacity = _count;
    categories = new const ProductCategory * [_count];
    for (size_t i = 0; i < _count; i++)
    {
        categories[i] = _categories[i]; 
    }
}

MultipleCategoryGiftCard::~MultipleCategoryGiftCard()
{
    free();
}

MultipleCategoryGiftCard::MultipleCategoryGiftCard(const MultipleCategoryGiftCard& other) : GiftCard(other)
{
    copyFrom(other);
}

MultipleCategoryGiftCard& MultipleCategoryGiftCard::operator=(const MultipleCategoryGiftCard& other)
{
    if (this != &other) 
    {
        GiftCard::operator=(other); 
        free();
        copyFrom(other);
    }
    return *this;
}

double MultipleCategoryGiftCard::apply(const CartItem* cart, size_t cartCount) const
{
    double totalDiscountBase = 0;
    for (size_t i = 0; i < cartCount; i++)
    {
        bool categoryMatch = false;
        for (size_t k = 0; k < categoriesCount; k++)
        {
            if (cart[i].categoryId == categories[k]->getId())
            {
                categoryMatch = true;
                break;
            }
        }

        if (categoryMatch) 
        {
            totalDiscountBase += (cart[i].pricePerItem * cart[i].quantity);
        }
    }
    return totalDiscountBase * (percentage / 100.0);
}

GiftCard* MultipleCategoryGiftCard::clone() const 
{
    return new MultipleCategoryGiftCard(*this);
}

void MultipleCategoryGiftCard::serialize(std::ostream& os) const 
{
    GiftCard::serialize(os);
    os << categoriesCount << ' ';
    for (size_t i = 0; i < categoriesCount; i++) {
        os << categories[i]->getId() << ' ';
    }
}