#include "pch.h"
#include "../RemoteConsole/server_executor.h"
#include "../RemoteConsole/marshaller.h"

using testing::_;
using testing::Return;

//class ServerExecutorMock : public ServerExecutor
//{
//public:
//	MOCK_METHOD0(initialize, bool());
//	MOCK_METHOD1(execute, void(const std::wstring&));
//	MOCK_CONST_METHOD0(getResult, std::wstring());
//
//	MOCK_METHOD0(send_error_message, void());
//	MOCK_CONST_METHOD0(get_last_error, DWORD());
//	MOCK_CONST_METHOD3(create_sub_process,
//					   bool(PROCESS_INFORMATION&, STARTUPINFOW&, wchar_t*));
//
//	void delegateToFake_create_sub_process()
//	{
//		ON_CALL(*this, create_sub_process(_, _, _))
//			.WillByDefault(Return(false));
//	}
//};
//
//
//
//
//
//TEST(ServerExecutorTest, IsCalled_create_sub_process_OnceIfIsInitialized)
//{
//	ServerExecutorMock mock;
//
//	if (mock.ServerExecutor::initialize())
//	{
//		EXPECT_CALL(mock, create_sub_process(_, _, _)).Times(1);
//
//		mock.ServerExecutor::execute(L"help");
//	}
//}
//
//TEST(ServerExecutorTest, IsCalled_send_error_message_OnceIfCannotCreateSubProcess)
//{
//	ServerExecutorMock mock;
//	mock.delegateToFake_create_sub_process();
//
//	if (mock.ServerExecutor::initialize())
//	{
//		EXPECT_CALL(mock, create_sub_process(_, _, _)).Times(1);
//		EXPECT_CALL(mock, send_error_message()).Times(1);
//
//		mock.ServerExecutor::execute(L"help");
//	}
//}


TEST(MarshallerTest, GetMode_Valid)
{
	int index = 0;
	std::wstring input(Marshaller::MODE[index] + std::wstring(L"text"));
	Marshaller::ModeIndex mode = Marshaller::getMode(input);

	EXPECT_EQ(mode, static_cast<Marshaller::ModeIndex>(index));
}

TEST(MarshallerTest, GetMode_Invalid_InvalidMode)
{
	std::wstring input(L'*' + std::wstring(L"text"));
	Marshaller::ModeIndex mode = Marshaller::getMode(input);

	EXPECT_EQ(mode, Marshaller::ModeIndex::WrongModeSymbol);
}

TEST(MarshallerTest, GetMode_Invalid_NoMode)
{
	std::wstring input(L"text");
	Marshaller::ModeIndex mode = Marshaller::getMode(input);

	EXPECT_EQ(mode, Marshaller::ModeIndex::WrongModeSymbol);
}

TEST(MarshallerTest, GetMode_Invalid_Empty)
{
	std::wstring input;
	Marshaller::ModeIndex mode = Marshaller::getMode(input);

	EXPECT_EQ(mode, Marshaller::ModeIndex::WrongModeSymbol);
}




TEST(MarshallerTest, Pack_ModeAuthorization_Valid)
{
	int index = static_cast<int>(Marshaller::ModeIndex::Authorization);

	std::wstring
		login		= L"login",
		password	= L"password";

	std::wstring result =
		Marshaller::MODE[index] + login + Marshaller::SEPARATOR + password;

	std::wstring output =
		Marshaller::packAuthorizationData(std::make_pair(login, password));

	EXPECT_EQ(result, output);
}

TEST(MarshallerTest, Pack_ModeAuthorization_Empty)
{
	int index = static_cast<int>(Marshaller::ModeIndex::Authorization);

	std::wstring
		login,
		password;

	std::wstring result;
	result += Marshaller::MODE[index];
	result += Marshaller::SEPARATOR;

	std::wstring output =
		Marshaller::packAuthorizationData(std::make_pair(login, password));

	EXPECT_EQ(result, output);
}


TEST(MarshallerTest, Unpack_ModeAuthorization_Valid)
{
	int index = static_cast<int>(Marshaller::ModeIndex::Authorization);

	std::wstring
		login		= L"login",
		password	= L"password";

	std::wstring input =
		Marshaller::MODE[index] + login +
		Marshaller::SEPARATOR	+ password;

	auto result = std::make_pair(login, password);
	auto output = Marshaller::unpackAuthorizationData(input);

	EXPECT_EQ(result, output);
}

TEST(MarshallerTest, Unpack_ModeAuthorization_Inalid_EmptyLogin)
{
	int index = static_cast<int>(Marshaller::ModeIndex::Authorization);

	std::wstring
		login		= L"",
		password	= L"password";

	std::wstring input =
		Marshaller::MODE[index] + login +
		Marshaller::SEPARATOR + password;

	auto result = std::make_pair(login, password);
	auto output = Marshaller::unpackAuthorizationData(input);

	EXPECT_EQ(result, output);
}

TEST(MarshallerTest, Unpack_ModeAuthorization_Inalid_EmptyPassword)
{
	int index = static_cast<int>(Marshaller::ModeIndex::Authorization);

	std::wstring
		login		= L"login",
		password	= L"";

	std::wstring input =
		Marshaller::MODE[index] + login +
		Marshaller::SEPARATOR + password;

	auto result = std::make_pair(login, password);
	auto output = Marshaller::unpackAuthorizationData(input);

	EXPECT_EQ(result, output);
}

TEST(MarshallerTest, Unpack_ModeAuthorization_Inalid_EmptyLoginPassword)
{
	int index = static_cast<int>(Marshaller::ModeIndex::Authorization);

	std::wstring
		login		= L"",
		password	= L"";

	std::wstring input =
		Marshaller::MODE[index] + login +
		Marshaller::SEPARATOR + password;

	auto result = std::make_pair(login, password);
	auto output = Marshaller::unpackAuthorizationData(input);

	EXPECT_EQ(result, output);
}


TEST(MarshallerTest, PackAndUnpack_ModeAuthorization_Valid)
{
	std::wstring
		login		= L"login",
		password	= L"password";

	std::wstring input =
		Marshaller::packAuthorizationData(std::make_pair(login, password));

	auto result = std::make_pair(login, password);
	auto output = Marshaller::unpackAuthorizationData(input);

	EXPECT_EQ(result, output);
}

TEST(MarshallerTest, PackAndUnpack_ModeAuthorization_Empty)
{
	std::wstring
		login		= L"",
		password	= L"";

	std::wstring input =
		Marshaller::packAuthorizationData(std::make_pair(login, password));

	auto result = std::make_pair(login, password);
	auto output = Marshaller::unpackAuthorizationData(input);

	EXPECT_EQ(result, output);
}




TEST(MarshallerTest, Pack_ModeCommand_Valid)
{
	int index = static_cast<int>(Marshaller::ModeIndex::Command);

	std::wstring command = L"dir";
	std::wstring result = Marshaller::MODE[index] + command;

	std::wstring output =
		Marshaller::packMessage(Marshaller::ModeIndex::Command, command);

	EXPECT_EQ(result, output);
}

TEST(MarshallerTest, Pack_ModeCommand_Empty)
{
	int index = static_cast<int>(Marshaller::ModeIndex::Command);

	std::wstring command = L"";
	std::wstring result = Marshaller::MODE[index] + command;

	std::wstring output =
		Marshaller::packMessage(Marshaller::ModeIndex::Command, command);

	EXPECT_EQ(result, output);
}


TEST(MarshallerTest, Unpack_ModeCommand_Valid)
{
	int index = static_cast<int>(Marshaller::ModeIndex::Command);

	std::wstring command = L"dir";

	std::wstring input =
		Marshaller::MODE[index] + command;

	auto output =
		Marshaller::unpackMessage(Marshaller::ModeIndex::Command, input);

	EXPECT_EQ(command, output);
}

TEST(MarshallerTest, Unpack_ModeCommand_Inalid_Empty)
{
	int index = static_cast<int>(Marshaller::ModeIndex::Command);

	std::wstring command = L"";

	std::wstring input =
		Marshaller::MODE[index] + command;

	auto output =
		Marshaller::unpackMessage(Marshaller::ModeIndex::Command, input);

	EXPECT_EQ(command, output);
}


TEST(MarshallerTest, PackAndUnpack_ModeCommand_Valid)
{
	std::wstring command = L"dir";

	std::wstring input =
		Marshaller::packMessage(Marshaller::ModeIndex::Command, command);

	auto output =
		Marshaller::unpackMessage(Marshaller::ModeIndex::Command, input);

	EXPECT_EQ(command, output);
}

TEST(MarshallerTest, PackAndUnpack_ModeCommand_Empty)
{
	std::wstring command = L"";

	std::wstring input =
		Marshaller::packMessage(Marshaller::ModeIndex::Command, command);

	auto output =
		Marshaller::unpackMessage(Marshaller::ModeIndex::Command, input);

	EXPECT_EQ(command, output);
}




TEST(MarshallerTest, Pack_ModeResult_Valid)
{
	int index = static_cast<int>(Marshaller::ModeIndex::Result);
	bool state = true;

	std::wstring result =	Marshaller::MODE[index] +
							std::wstring{ state ? L'1' : L'0' };

	std::wstring output =
		Marshaller::packResult(state);

	EXPECT_EQ(result, output);
}


TEST(MarshallerTest, Unpack_ModeResult_Valid)
{
	int index = static_cast<int>(Marshaller::ModeIndex::Result);
	bool state = true;

	std::wstring input = Marshaller::MODE[index] +
							std::wstring{ state ? L'1' : L'0' };

	bool result = Marshaller::unpackResult(input);

	EXPECT_EQ(state, result);
}


TEST(MarshallerTest, PackAndUnpack_ModeResult_Valid)
{
	bool state = true;

	std::wstring	input	= Marshaller::packResult(state);
	bool			result	= Marshaller::unpackResult(input);

	EXPECT_EQ(state, result);
}
