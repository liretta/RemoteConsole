#include "server_executor.h"

#include <iostream>

static wchar_t* ANSItoUNICODE(char* line)
{
	int
		length = int(strlen(line)) + 1,
		size_needed = MultiByteToWideChar(CP_OEMCP, 0, line, length, nullptr, 0);

	// allocate memory for the Unicode line
	wchar_t* w_line =
		static_cast<wchar_t*>(LocalAlloc(LPTR, sizeof(wchar_t) * size_needed));

	// transform
	MultiByteToWideChar(CP_OEMCP, 0, line, length, w_line, size_needed);

	return w_line;
}

static std::wstring COMMANDLINEtoCMDCOMMAND(const std::wstring& command_line)
{
	std::wstring cmd_command(L"/C ");
	cmd_command.append(command_line);
	return cmd_command;
}


bool ServerExecutor::initialize()
{
	m_security_attributes.nLength = sizeof(SECURITY_ATTRIBUTES);
	m_security_attributes.bInheritHandle = TRUE;
	m_security_attributes.lpSecurityDescriptor = nullptr;

	bool is_initialized = true;

	if (CreatePipe(&m_child_out_read,
	               &m_child_out_write, &m_security_attributes, 0) != TRUE)
	{
		is_initialized = false;
	}

	// ensure the read handle to the pipe for STDOUT is not inherited
	if (SetHandleInformation(m_child_out_read,
	                         HANDLE_FLAG_INHERIT, 0) != TRUE)
	{
		is_initialized = false;
	}

	m_is_initialized = is_initialized;

	return is_initialized;
}


void ServerExecutor::execute(const std::wstring& command)
{
	std::wstring cmd_command = COMMANDLINEtoCMDCOMMAND(command);

	wchar_t w_command[MAX_PATH];
	wcscpy_s(w_command, cmd_command.data());


	PROCESS_INFORMATION process_information;
	STARTUPINFOW		startup_info;

	bool is_created = create_sub_process(process_information,
										 startup_info, w_command);

	if (!is_created)
	{
		send_error_message();
	}
	else
	{
		CloseHandle(process_information.hProcess);
		CloseHandle(process_information.hThread);
	}
}


std::wstring ServerExecutor::getResult() const
{
	std::wstring result;

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

		LocalFree(w_tmp_buffer);
	}
	while (is_read == TRUE && read_symbols_count != 0);

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
		case 2:
			message = "Invalid command";
			break;
		case 6:
			message = "Invalid server process";
			break;
		default:
			message = "Unhandled error";
			break;
		}

		// cannot pass nullptr, so need a variable
		DWORD written_symbols_count;
		BOOL is_written = WriteFile(
			m_child_out_write, message.data(),
			DWORD(message.size()) + 1, &written_symbols_count, nullptr
		);

		if (!is_written)
		{
			std::cerr << "ERROR: " << GetLastError() << "cannot write error message to pipe\n";
		}
	}
}

bool ServerExecutor::create_sub_process(
	PROCESS_INFORMATION& process_info,
	STARTUPINFOW& startup_info, wchar_t* w_command) const
{
	ZeroMemory(&process_info, sizeof(PROCESS_INFORMATION));

	ZeroMemory(&startup_info, sizeof(STARTUPINFOW));
	startup_info.cb = sizeof(STARTUPINFOW);
	startup_info.hStdError = m_child_out_write;
	startup_info.hStdOutput = m_child_out_write;
	startup_info.hStdInput = nullptr;
	startup_info.dwFlags |= STARTF_USESTDHANDLES;

	// create a child process
	BOOL is_created = CreateProcessW(
		L"C:\\Windows\\System32\\cmd.exe",
		w_command,
		nullptr,
		nullptr,
		TRUE,
		0,
		nullptr,
		nullptr,
		&startup_info,
		&process_info
	);

	return is_created == TRUE;
}
