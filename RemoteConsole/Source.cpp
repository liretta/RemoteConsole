#include "server_executor.h"
#include <string>
#include <iostream>

int main()
{
	ServerExecutor executor;
	if (executor.initialize())
	{
		executor.execute(std::wstring(L"dir"));

		auto text = executor.getResult();
		std::wcout << text.size() << '\n' << text << std::endl;
	}
	else
	{
		std::cerr << "cannot initialize executor" << std::endl;
	}

	

	system("PAUSE");
	return 0;
}
