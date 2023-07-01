#pragma once

#include <iostream>
#include <sstream>
#include <stdio.h>

template<class T> using TSharedPtr = std::shared_ptr<T>;

template<typename T>
std::string toString(const T& value)
{
	std::ostringstream s;
	s << value;
	return s.str();
}

