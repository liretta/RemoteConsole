//TODO: add cryptor
#include "client_executor.h"

ClientExecutor::ClientExecutor(ClientNetworker& networker):
    m_networker(networker)
{
}

std::wstring ClientExecutor::execute(const std::wstring& w_command)
{
    std::wstring execution_result;

    std::wstring w_message =
        Marshaller::packMessage(Marshaller::Type::Command, w_command);

    if (m_networker.send(WSTRINGtoSTRING(w_message)))
    {
        std::string result = m_networker.receive();
        w_message = STRINGtoWSTRING(result);

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
    else
    {
        std::cerr << "ERROR: unable to send data to server" << std::endl;
    }

    return execution_result;
}
