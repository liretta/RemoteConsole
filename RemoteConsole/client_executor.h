#pragma once

#include "marshaller.h"
#include "client_networker.h"
#include  "client_cryptor.h"
#include "converting_functions.hpp"

#include <string>
#include <iostream>

class ClientExecutor
{
public:

    ClientExecutor(ClientNetworker& networker, ClientCryptor& cryptor);

    std::wstring execute(const std::wstring& w_command);

private:

    ClientNetworker& m_networker;
	ClientCryptor& m_cryptor;

};
