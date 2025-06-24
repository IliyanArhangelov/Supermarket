#include "GiftCard.h"

GiftCard::GiftCard(const char* _code, double _percentage, GiftCardType _type) : type(_type)
{
    code = nullptr;
    StrOpr::strCopy(code, _code);
    percentage = (_percentage > 0 && _percentage <= 100) ? _percentage : 0;
}

GiftCard::GiftCard(const GiftCard& other)
{
    code = nullptr;
    StrOpr::strCopy(code, other.code);
    percentage = other.percentage;
    type = other.type;
}

GiftCard& GiftCard::operator=(const GiftCard& other)
{
    if (this != &other) 
    {
        delete[] code;
        code = nullptr;
        StrOpr::strCopy(code, other.code);
        percentage = other.percentage;
        type = other.type;
    }
    return *this;
}

GiftCard::~GiftCard() 
{
    delete[] code;
}

GiftCard::GiftCard(GiftCard&& other) noexcept 
{
    code = other.code;
    percentage = other.percentage;
    type = other.type;
    other.code = nullptr;
}

GiftCard& GiftCard::operator=(GiftCard&& other) noexcept 
{
    if (this != &other) 
    {
        delete[] code;
        code = other.code;
        percentage = other.percentage;
        type = other.type;
        other.code = nullptr;
    }
    return *this;
}


double GiftCard::getPercentage() const
{
    return percentage;
}

const char* GiftCard::getCode() const
{
    return code;
}

void GiftCard::serialize(std::ostream& os) const
{
    os << static_cast<int>(type) << ' ';
    FileOpr::writeString(os, code);
    os << percentage << ' ';
}
