#include "server_executor.h"

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
	return std::wstring(L"cmd /C ") + command_line;
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

	return is_initialized;
}


void ServerExecutor::execute(const std::wstring& command)
{
	std::wstring cmd_command = COMMANDLINEtoCMDCOMMAND(command);

	wchar_t w_command[MAX_PATH];
	wcscpy_s(w_command, cmd_command.data());

	PROCESS_INFORMATION process_information;
	ZeroMemory(&process_information, sizeof(PROCESS_INFORMATION));

	STARTUPINFOW startup_info;
	ZeroMemory(&startup_info, sizeof(STARTUPINFOW));
	// set up members of the STARTUPINFOW structure
	// this structure specifies the STDIN and STDOUT handles for redirection
	startup_info.cb = sizeof(STARTUPINFOW);
	startup_info.hStdError = m_child_out_write;
	startup_info.hStdOutput = m_child_out_write;
	startup_info.hStdInput = nullptr;
	startup_info.dwFlags |= STARTF_USESTDHANDLES;

	// create the child process
	BOOL is_created = CreateProcessW(
		nullptr,
		w_command,				// command line 
		nullptr,				// process security attributes 
		nullptr,				// primary thread security attributes 
		TRUE,					// handles are inherited 
		0,						// creation flags 
		nullptr,				// use parent's environment 
		nullptr,				// use parent's current directory 
		&startup_info,			// STARTUPINFOW pointer 
		&process_information	// receives PROCESS_INFORMATION 
	);

	if (is_created != TRUE)
	{
		send_error_message();
	}
	else
	{
		// close handles to the child process and its primary thread;
		// some applications might keep these handles to monitor the status
		// of the child process, for example
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
		default:
			message = "Unhandled error";
			break;
		}

		DWORD written_symbols_count; //< cannot pass nullptr, so need a variable
		WriteFile(m_child_out_write, message.data(),
				  DWORD(message.size()) + 1, &written_symbols_count, nullptr);
	}
}
