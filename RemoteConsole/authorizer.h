#pragma once
#include <string>


class Authorizer
{
public:

	bool authorize(const std::pair<std::wstring, std::wstring>& data)
	{
		return true;
	}
};
