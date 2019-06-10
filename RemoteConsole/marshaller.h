#pragma once

#include <string>
#include <vector>

/*!
 * Emptiness of wstring is not checked by all functions.
 */
class Marshaller
{
public:

    enum class Type : int
    {
        Authorization,
        Command,
        Error,
        Result,

        WrongModeSymbol
    };


    static Type getMode(const std::wstring& input);

    static std::pair<std::wstring, std::wstring>
    unpackAuthorizationData(const std::wstring&);

    static std::wstring
    packAuthorizationData(const std::wstring&, const std::wstring&);


    static std::wstring unpackMessage(Type, const std::wstring&);
    static std::wstring packMessage(Type, const std::wstring&);


    static bool         unpackResult(const std::wstring&);
    static std::wstring packResult(bool);

//private:

    static const wchar_t MODE[];
    static const wchar_t SEPARATOR;
};
