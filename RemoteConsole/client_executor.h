#pragma once

#include "marshaller.h"
#include "client_networker.h"
#include "converting_functions.hpp"

#include <string>
#include <iostream>

class ClientExecutor
{
public:

    ClientExecutor(ClientNetworker& networker);

    std::wstring execute(const std::wstring& w_command);

private:

    ClientNetworker& m_networker;
};
