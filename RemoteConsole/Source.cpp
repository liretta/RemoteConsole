#include <windows.h> 
#include <tchar.h>
#include <stdio.h> 
#include <strsafe.h>

#include "server_executor.h"

int main()
{
	ServerExecutor executor;
	if (executor.initialize())
	{
		if (executor.startProcess())
		{
			executor.execute(L"help");
			std::wcout << executor.getResult();
		}
	}
	else
	{
		std::cout << "Cannot initialize Executor\n";
	} 

	return 0;
}
