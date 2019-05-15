#include "marshaller.h"
#include <iostream>
#include <regex>

const wchar_t Marshaller::MODE[]{ L'L', L'C', L'E', L'R', L'#' };
const wchar_t Marshaller::SEPARATOR = L'|';

/*!
 * Need to get Marshaller MODE size.
 */
template <typename  T, std::size_t N>
static constexpr  std::size_t ARRAYSIZE(T(&)[N]) noexcept
{
	return N;
}


Marshaller::ModeIndex Marshaller::getMode(const std::wstring& input)
{
	wchar_t		mode_symbol	= input[0];
	int			mode_size	= int(ARRAYSIZE(MODE));

	int i = 0;
	while (i < mode_size && MODE[i] != mode_symbol)
	{
		++i;
	}

	ModeIndex result = ModeIndex::WrongModeSymbol;
	if (i != mode_size)
	{
		result = static_cast<ModeIndex>(i);
	}
	else
	{
		std::cerr << "ERROR: wrong mode symbol " << mode_symbol << std::endl;
	}
	return result;
}

std::pair<std::wstring, std::wstring>
Marshaller::unpackAuthorizationData(const std::wstring& w_line)
{
	// template: Llogin|password

	static std::wstring pattern =
		MODE[static_cast<int>(ModeIndex::Authorization)] +
		std::wstring(L"([[:alnum:]])|([[:alnum:]])");

	std::wregex		regex(pattern);
	std::wsmatch	results;

	std::wstring login, password;
	if (std::regex_search(w_line, results, regex))
	{
		login		= results.str(1);
		password	= results.str(2);
	}

	return std::make_pair(login, password);
}

std::wstring
Marshaller::packAuthorizationData(const std::pair<std::wstring, std::wstring>& data)
{
	std::wstring result;
	result += MODE[static_cast<int>(ModeIndex::Authorization)];
	result += data.first;
	result += SEPARATOR;
	result += data.second;

	return result;
}

std::wstring Marshaller::unpackMessage(ModeIndex mode, const std::wstring& w_line)
{
	// template: Mmessage
	static std::wstring pattern =
		MODE[static_cast<int>(mode)] +
		std::wstring(L"([[:print:]])");

	std::wregex		regex(pattern);
	std::wsmatch	results;

	std::wstring message;
	if (std::regex_search(w_line, results, regex))
	{
		message = results.str(1);
	}

	return message;
}

std::wstring Marshaller::packMessage(ModeIndex mode, const std::wstring& w_line)
{
	std::wstring result;
	result += MODE[static_cast<int>(mode)];
	result += w_line;

	return result;
}

bool Marshaller::unpackResult(const std::wstring& w_line)
{
	static std::wstring pattern =
		MODE[static_cast<int>(ModeIndex::Result)] +
		std::wstring(L"(0|1)");

	std::wregex		regex(pattern);
	std::wsmatch	results;

	bool result = false;
	if (std::regex_search(w_line, results, regex))
	{
		result = (results.str(1) == L"1");
	}

	return result;
}

std::wstring Marshaller::packResult(bool value)
{
	std::wstring result;
	result += MODE[static_cast<int>(ModeIndex::Result)];
	result += (value ? L'1' : L'0');

	return result;
}
