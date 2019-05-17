#include "pch.h"

#include "../RemoteConsole/server_router.h"

TEST(ServerRouter, process_ReturnEmptyWstringIfParamIsEmptyWstring)
{
	ServerRouter router;

	std::wstring
		input,
		output = router.process(input);

	EXPECT_EQ(input, output);
}

TEST(ServerRouter, process_ReturnEmptyWstringIfParamModeIsWrong)
{
	ServerRouter router;

	std::wstring
		result,
		input = L"text",
		output = router.process(input);

	EXPECT_EQ(result, output);
}

TEST(ServerRouter, process_ReturnEmptyWstringIfParamModeIsError)
{
	int index = static_cast<int>(Marshaller::Type::Error);
	ServerRouter router;

	std::wstring
		command	= L"here is an error",
		input	= Marshaller::packMessage(Marshaller::Type::Error, command),
		output	= router.process(input);

	EXPECT_EQ(std::wstring(), output);
}