#pragma once

#include <string>

class IFormattable
{
public:
	virtual std::string get() = 0;
};