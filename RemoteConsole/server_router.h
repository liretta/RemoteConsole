#pragma once
#include "marshaller.h"
#include "server_executor.h"

class Authorizer
{
public:

    bool authorize(const std::wstring&, const std::wstring&)
    {
        return true;
    }
};

class ServerRouter
{
public:

    std::wstring process(const std::wstring& input);

private:

    static void     process_error_message(const std::wstring& input);

    std::wstring    process_authorization(const std::wstring& input);
    std::wstring    process_execution(const std::wstring& input);
    

    void initialize_authorizer();
    void initialize_executor();

    std::unique_ptr<Authorizer>         m_authorizer;
    std::unique_ptr<ServerExecutor>     m_executor;
};
