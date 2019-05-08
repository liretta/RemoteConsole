#include <string>
#include <iostream>

#include "server_executor.h"

int main()
{
	ServerExecutor executor;
	if (executor.initialize())
	{
		executor.execute(std::wstring(L"help"));

		auto text = executor.getResult();
	}
	else
	{
		std::cerr << "cannot initialize executor" << std::endl;
	}

	return 0;
}
