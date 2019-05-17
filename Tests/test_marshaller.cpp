#include "pch.h"

#include "../RemoteConsole/marshaller.h"




TEST(MarshallerTest, GetMode_Valid)
{
	int index = 0;
	std::wstring input(Marshaller::MODE[index] + std::wstring(L"text"));
	Marshaller::Type mode = Marshaller::getMode(input);

	EXPECT_EQ(mode, static_cast<Marshaller::Type>(index));
}

TEST(MarshallerTest, GetMode_Invalid_InvalidMode)
{
	std::wstring input(L'*' + std::wstring(L"text"));
	Marshaller::Type mode = Marshaller::getMode(input);

	EXPECT_EQ(mode, Marshaller::Type::WrongModeSymbol);
}

TEST(MarshallerTest, GetMode_Invalid_NoMode)
{
	std::wstring input(L"text");
	Marshaller::Type mode = Marshaller::getMode(input);

	EXPECT_EQ(mode, Marshaller::Type::WrongModeSymbol);
}

TEST(MarshallerTest, GetMode_Invalid_Empty)
{
	std::wstring input;
	Marshaller::Type mode = Marshaller::getMode(input);

	EXPECT_EQ(mode, Marshaller::Type::WrongModeSymbol);
}




TEST(MarshallerTest, Pack_ModeAuthorization_Valid)
{
	int index = static_cast<int>(Marshaller::Type::Authorization);

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
	int index = static_cast<int>(Marshaller::Type::Authorization);

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
	int index = static_cast<int>(Marshaller::Type::Authorization);

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
	int index = static_cast<int>(Marshaller::Type::Authorization);

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
	int index = static_cast<int>(Marshaller::Type::Authorization);

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
	int index = static_cast<int>(Marshaller::Type::Authorization);

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
	int index = static_cast<int>(Marshaller::Type::Command);

	std::wstring command = L"dir";
	std::wstring result = Marshaller::MODE[index] + command;

	std::wstring output =
		Marshaller::packMessage(Marshaller::Type::Command, command);

	EXPECT_EQ(result, output);
}

TEST(MarshallerTest, Pack_ModeCommand_Empty)
{
	int index = static_cast<int>(Marshaller::Type::Command);

	std::wstring command = L"";
	std::wstring result = Marshaller::MODE[index] + command;

	std::wstring output =
		Marshaller::packMessage(Marshaller::Type::Command, command);

	EXPECT_EQ(result, output);
}


TEST(MarshallerTest, Unpack_ModeCommand_Valid)
{
	int index = static_cast<int>(Marshaller::Type::Command);

	std::wstring command = L"dir";

	std::wstring input =
		Marshaller::MODE[index] + command;

	auto output =
		Marshaller::unpackMessage(Marshaller::Type::Command, input);

	EXPECT_EQ(command, output);
}

TEST(MarshallerTest, Unpack_ModeCommand_Inalid_Empty)
{
	int index = static_cast<int>(Marshaller::Type::Command);

	std::wstring command = L"";

	std::wstring input =
		Marshaller::MODE[index] + command;

	auto output =
		Marshaller::unpackMessage(Marshaller::Type::Command, input);

	EXPECT_EQ(command, output);
}


TEST(MarshallerTest, PackAndUnpack_ModeCommand_Valid)
{
	std::wstring command = L"dir";

	std::wstring input =
		Marshaller::packMessage(Marshaller::Type::Command, command);

	auto output =
		Marshaller::unpackMessage(Marshaller::Type::Command, input);

	EXPECT_EQ(command, output);
}

TEST(MarshallerTest, PackAndUnpack_ModeCommand_Empty)
{
	std::wstring command = L"";

	std::wstring input =
		Marshaller::packMessage(Marshaller::Type::Command, command);

	auto output =
		Marshaller::unpackMessage(Marshaller::Type::Command, input);

	EXPECT_EQ(command, output);
}




TEST(MarshallerTest, Pack_ModeResult_Valid)
{
	int index = static_cast<int>(Marshaller::Type::Result);
	bool state = true;

	std::wstring result =	Marshaller::MODE[index] +
							std::wstring{ state ? L'1' : L'0' };

	std::wstring output =
		Marshaller::packResult(state);

	EXPECT_EQ(result, output);
}


TEST(MarshallerTest, Unpack_ModeResult_Valid)
{
	int index = static_cast<int>(Marshaller::Type::Result);
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
