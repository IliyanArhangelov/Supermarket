#pragma once
#include "Constants.h"
#include "CartItem.h"
#include "Product.h"
#include <iostream>

class GiftCard
{
protected:
	char* code;
	double percentage;
	GiftCardType type;

public:
	GiftCard(const char* _code, double _percentage, GiftCardType _type);

	GiftCard(const GiftCard& other);
	GiftCard& operator=(const GiftCard& other);
	virtual ~GiftCard();
	GiftCard(GiftCard&& other) noexcept;
	GiftCard& operator=(GiftCard&& other) noexcept;

	double getPercentage() const;
	const char* getCode() const;

	virtual double apply(const CartItem* cart, size_t cartCount) const = 0;
	virtual GiftCard* clone() const = 0;
	virtual void serialize(std::ostream& os) const;
};
