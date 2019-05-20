#include "marshaller.h"
#include <iostream>
#include <regex>

const wchar_t Marshaller::MODE[]{ L'A', L'C', L'E', L'R', L'0' };
const wchar_t Marshaller::SEPARATOR = L'|';

/*!
 * Need to get Marshaller MODE size.
 */
template <typename  T, std::size_t N>
static constexpr  std::size_t ARRAYSIZE(T(&)[N]) noexcept
{
    return N;
}


Marshaller::Type Marshaller::getMode(const std::wstring& input)
{
    wchar_t     mode_symbol = input[0];
    int         mode_size   = int(ARRAYSIZE(MODE));

    int i = 0;
    while (i < mode_size && MODE[i] != mode_symbol)
    {
        ++i;
    }

    Type result = Type::WrongModeSymbol;
    if (i != mode_size)
    {
        result = static_cast<Type>(i);
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
        MODE[static_cast<int>(Type::Authorization)] +
        std::wstring(L"([[:alnum:]]*)\\|([[:alnum:]]*)");

    std::wregex     regex(pattern);
    std::wsmatch    results;

    std::wstring login, password;
    if (std::regex_search(w_line, results, regex))
    {
        login       = results.str(1);
        password    = results.str(2);
    }

    return std::make_pair(login, password);
}

std::wstring
Marshaller::packAuthorizationData(const std::wstring& login,
                                  const std::wstring& password)
{
    std::wstring result;
    result += MODE[static_cast<int>(Type::Authorization)];
    result += login;
    result += SEPARATOR;
    result += password;

    return result;
}

std::wstring Marshaller::unpackMessage(Type mode, const std::wstring& w_line)
{
    // template: Mmessage
    std::wstring message;

    if (w_line[0] == MODE[static_cast<int>(mode)])
    {
        message = std::wstring(w_line, 1, w_line.size() - 1);
    }

    return message;
}

std::wstring Marshaller::packMessage(Type mode, const std::wstring& w_line)
{
    std::wstring result;
    result += MODE[static_cast<int>(mode)];
    result += w_line;

    return result;
}

bool Marshaller::unpackResult(const std::wstring& w_line)
{
    static std::wstring pattern =
        MODE[static_cast<int>(Type::Result)] +
        std::wstring(L"(0|1)");

    std::wregex     regex(pattern);
    std::wsmatch    results;

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
    result += MODE[static_cast<int>(Type::Result)];
    result += (value ? L'1' : L'0');

    return result;
}
