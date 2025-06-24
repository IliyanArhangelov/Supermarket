#pragma once
#include "models/worker/Worker.h"

namespace Logger
{
	void log(const char* message, const Worker* user = nullptr);
}