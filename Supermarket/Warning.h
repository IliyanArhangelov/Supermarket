#pragma once
#include "Constants.h"

class Warning
{
private:
	char* senderName;
	char* description;
	WarningLevel level;

	void free();
	void copyFrom(const Warning& other);
	void moveFrom(Warning&& other) noexcept;

public:
	Warning() = delete;
	Warning(const char* sender, const char* desc, WarningLevel lvl);
	Warning(const Warning& other);
	Warning(Warning&& other) noexcept;
	Warning& operator=(const Warning& other);
	Warning& operator=(Warning&& other) noexcept;
	~Warning();

	const char* getSender() const;
	const char* getDescription() const;
	WarningLevel getLevel() const;

	void serialize(std::ostream& os) const;
};