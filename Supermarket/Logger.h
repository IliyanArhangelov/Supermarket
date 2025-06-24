#pragma once
#include "Worker.h"

namespace Logger
{
	void log(const char* message, const Worker* user = nullptr);
}