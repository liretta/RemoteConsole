#include "client_executor.h"

ClientExecutor::ClientExecutor(ClientNetworker& networker, ClientCryptor& cryptor):
    m_networker(networker), m_cryptor(cryptor)
{
}

std::wstring ClientExecutor::execute(const std::wstring& w_command)
{
    std::wstring execution_result;

    std::wstring w_message =
        Marshaller::packMessage(Marshaller::Type::Command, w_command);

    if (m_networker.send(m_cryptor.encrypt(w_message)))
    {
		std::vector<char> vc;
		bool result = m_networker.receive(vc);
		if (result)
		{
			std::wstring w_message = m_cryptor.decrypt(vc);

			switch (Marshaller::getMode(w_message))
			{
			case Marshaller::Type::Command:
			{
				execution_result =
					Marshaller::unpackMessage(Marshaller::Type::Command,
						w_message);
				break;
			}
			case Marshaller::Type::Error:
			{
				execution_result =
					Marshaller::unpackMessage(Marshaller::Type::Error,
						w_message);
				break;
			}
			default:
			{
				std::wcerr << L"ERROR: unable to unpack " <<
					w_message << std::endl;
			}
			}
		}
    }
    else
    {
        std::wcerr << L"ERROR: unable to send data to server" << std::endl;
    }

    return execution_result;
}
