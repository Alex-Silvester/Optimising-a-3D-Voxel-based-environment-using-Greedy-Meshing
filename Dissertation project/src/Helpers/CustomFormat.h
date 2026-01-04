#pragma once

#include "IFormattable.h"

void print(IFormattable& formattable, bool use_next_line)
{
	std::cout << formattable.get() << (use_next_line ? "\n" : "");
}

void println(IFormattable& formattable)
{
	print(formattable, true);
}