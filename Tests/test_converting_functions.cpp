#include "pch.h"

#include "../RemoteConsole/converting_functions.hpp"

TEST(ConvertingFunctions, WstringToStringAndStringToWstring)
{
	std::wstring	w_str	= L"wide text";
	std::string		str		= WSTRINGtoSTRING(w_str);
	std::wstring	result	= STRINGtoWSTRING(str);

	EXPECT_EQ(w_str, result);
}

TEST(ConvertingFunctions, StringToWstringAndWstringToString)
{
	std::string		str		= "char text";
	std::wstring	w_str	= STRINGtoWSTRING(str);
	std::string		result	= WSTRINGtoSTRING(w_str);

	EXPECT_EQ(str, result);
}