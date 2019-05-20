#pragma once

#include "marshaller.h"
#include "converting_functions.hpp"

#include <string>
#include <iostream>

class Networker
{
public:

    bool send(const std::string&);
    std::string receive();
};


class ClientExecutor
{
public:

    ClientExecutor(Networker& networker);

    std::wstring execute(const std::wstring& w_command);

private:

    Networker& m_networker;
};
