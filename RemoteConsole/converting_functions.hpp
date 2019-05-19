#pragma once
#include <string>
#include <windows.h>
#include <locale>
#include <codecvt>

inline wchar_t* ANSItoUNICODE(char* line)
{
	int
		length = int(strlen(line)) + 1,
		size_needed = MultiByteToWideChar(CP_OEMCP, 0, line, length, nullptr, 0);

	// allocate memory for the Unicode line
	auto* w_line = new wchar_t[sizeof(wchar_t) * size_needed];

	// transform
	MultiByteToWideChar(CP_OEMCP, 0, line, length, w_line, size_needed);

	return w_line;
}

inline std::string WSTRINGtoSTRING(const std::wstring& w_str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.to_bytes(w_str);
}

inline std::wstring STRINGtoWSTRING(const std::string& str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(str);
}