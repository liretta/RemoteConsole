#include "server_executor.h"
#include <iostream>
#include "converting_functions.hpp"


static std::wstring toCONSOLECOMMAND(const std::wstring& command_line)
{
    return std::wstring(L"cmd /C ") + command_line;
}


ServerExecutor::ServerExecutor()
{
    m_security_attributes.nLength = sizeof(SECURITY_ATTRIBUTES);
    m_security_attributes.bInheritHandle = TRUE;
    m_security_attributes.lpSecurityDescriptor = nullptr;
}

bool ServerExecutor::initialize()
{

    m_is_initialized = true;

    if (CreatePipe(&m_child_out_read,
        &m_child_out_write, &m_security_attributes, 0) != TRUE)
    {
        m_is_initialized = false;
    }

    // ensure the read handle to the pipe for STDOUT is not inherited
    if (SetHandleInformation(m_child_out_read,
        HANDLE_FLAG_INHERIT, 0) != TRUE)
    {
        m_is_initialized = false;
    }

    return m_is_initialized;
}


bool ServerExecutor::execute(const std::wstring& command)
{
    bool is_executed = false;

    if (m_is_initialized)
    {
        std::wstring cmd_command = toCONSOLECOMMAND(command);

        wchar_t w_command[MAX_PATH];
        wcscpy_s(w_command, cmd_command.data());

        PROCESS_INFORMATION process_information;
        ZeroMemory(&process_information, sizeof(PROCESS_INFORMATION));

        is_executed = create_sub_process(process_information, w_command);

        if (!is_executed)
        {
            send_error_message();
        }
        else
        {
            CloseHandle(process_information.hProcess);
            CloseHandle(process_information.hThread);
        }
    }
    else
    {
        std::wcerr   << L"ERROR: cannot execute command; "
                    << L"ServerExecutor is not initialized"
                    << std::endl;
    }

    return is_executed;
}


std::wstring ServerExecutor::getResult() const
{
    std::wstring result;

    if (m_is_initialized)
    {
        DWORD read_symbols_count = 0;
        char buffer[BUFSIZE];
        BOOL is_read = FALSE;

        // close writing side of pipe to be able to read
        CloseHandle(m_child_out_write);

        wchar_t* w_tmp_buffer = nullptr;
        do
        {
            memset(buffer, 0, BUFSIZE);

            is_read = ReadFile(m_child_out_read, buffer,
                               BUFSIZE, &read_symbols_count, nullptr);

            w_tmp_buffer = ANSItoUNICODE(buffer);
            result.append(w_tmp_buffer);

            delete w_tmp_buffer;
        } while (is_read == TRUE && read_symbols_count != 0);

        CloseHandle(m_child_out_read);
    }
    else
    {
        std::wcerr   << L"ERROR: no result found; "
                    << L"ServerExecutor is not initialized"
                    << std::endl;
    }

    return result;
}

bool ServerExecutor::isInitialized() const
{
    return m_is_initialized;
}


void ServerExecutor::send_error_message()
{
    DWORD error = GetLastError();

    if (error != 0)
    {
        std::string message;

        switch (error)
        {
        case ERROR_FILE_NOT_FOUND:
            message = "Invalid command";
            break;
        default:
            message = "Unhandled error";
            break;
        }

        DWORD written_symbols_count; //< cannot pass nullptr, so need a variable
        WriteFile(m_child_out_write, message.data(),
                  DWORD(message.size()) + 1, &written_symbols_count, nullptr);
    }
}

bool ServerExecutor::create_sub_process(PROCESS_INFORMATION& process_info,
                                        wchar_t* w_command)
{
    STARTUPINFOW startup_info;
    ZeroMemory(&startup_info, sizeof(STARTUPINFOW));

    startup_info.cb = sizeof(STARTUPINFOW);
    startup_info.hStdError = m_child_out_write;
    startup_info.hStdOutput = m_child_out_write;
    startup_info.hStdInput = nullptr;
    startup_info.dwFlags |= STARTF_USESTDHANDLES;

    // create the child process
    BOOL is_created = CreateProcessW(
        nullptr,
        w_command,
        nullptr,
        nullptr,
        TRUE,
        CREATE_NO_WINDOW,
        nullptr,
        nullptr,
        &startup_info,
        &process_info
    );

    return is_created == TRUE;
}
