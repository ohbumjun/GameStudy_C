#pragma once
#include <iostream>

extern int g_value;

void hello()
{
	std::cout << "g_value : " << g_value << std::endl;
}