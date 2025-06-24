#include "Warning.h"
#include "utils/Constants.h"
#include <utility>


void Warning::free()
{
	delete[] senderName;
	delete[] description;
	senderName = nullptr;
	description = nullptr;
}

void Warning::copyFrom(const Warning& other)
{
	level = other.level;

	senderName = nullptr;
	description = nullptr;
	StrOpr::strCopy(senderName, other.senderName);
	StrOpr::strCopy(description, other.description);
}

void Warning::moveFrom(Warning&& other) noexcept
{
	level = other.level;
	senderName = other.senderName;
	description = other.description;

	other.senderName = nullptr;
	other.description = nullptr;
}

Warning::Warning(const char* sender, const char* desc, WarningLevel lvl)
{
	level = lvl;
	senderName = nullptr;
	description = nullptr;
	StrOpr::strCopy(senderName, sender);
	StrOpr::strCopy(description, desc);
}

Warning::~Warning()
{
	free();
}

Warning::Warning(const Warning& other)
{
	copyFrom(other);
}

Warning::Warning(Warning&& other) noexcept
{
	moveFrom(std::move(other));
}

Warning& Warning::operator=(const Warning& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}

Warning& Warning::operator=(Warning&& other) noexcept
{
	if (this != &other)
	{
		free();
		moveFrom(std::move(other));
	}
	return *this;
}

const char* Warning::getSender() const 
{ 
	return senderName;
}
const char* Warning::getDescription() const
{ 
	return description; 
}
WarningLevel Warning::getLevel() const
{ 
	return level;
}

void Warning::serialize(std::ostream& os) const
{
	FileOpr::writeString(os, senderName);
	FileOpr::writeString(os, description);
	os << static_cast<int>(level) << ' ';
}
