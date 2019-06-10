#include "pch.h"
#include "../RemoteConsole/server_executor.h"

using testing::_;
using testing::Return;
using testing::Invoke;


TEST(ServerExecutorTest, getResult_ReturnEmptyWstringIfNoActionsDone)
{
	ServerExecutor executor;
	std::wstring
		output = executor.getResult(),
		result;

	EXPECT_EQ(result, output);
}

TEST(ServerExecutorTest, execute_ReturnFalseIfNotInitialized)
{
	ServerExecutor executor;
	bool output = executor.execute(L"help");

	EXPECT_FALSE(output);
}

