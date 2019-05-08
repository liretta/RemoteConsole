#include "server_executor.h"

static wchar_t* ANSItoUNICODE(char* line)
{
	std::size_t
		length = strlen(line) + 1,
		size_needed = MultiByteToWideChar(CP_OEMCP, 0, line, length, nullptr, 0);

	wchar_t* w_line =
		static_cast<wchar_t*>(LocalAlloc(LPTR, sizeof(wchar_t) * size_needed));

	MultiByteToWideChar(CP_OEMCP, 0, line, length, w_line, size_needed);
	return w_line;
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

	// Ensure the read handle to the pipe for STDOUT is not inherited.
	if (SetHandleInformation(m_child_out_read,
	                         HANDLE_FLAG_INHERIT, 0) != TRUE)
	{
		is_initialized = false;
	}

	if (CreatePipe(&m_child_in_read,
				   &m_child_in_write, &m_security_attributes, 0) != TRUE)
	{
		is_initialized = false;
	}

	// Ensure the read handle to the pipe for STDOUT is not inherited.
	if (SetHandleInformation(m_child_out_write,
							 HANDLE_FLAG_INHERIT, 0) != TRUE)
	{
		is_initialized = false;
	}


	return is_initialized;
}

bool ServerExecutor::startProcess()
{
	bool is_created = true;

	WCHAR szCmdline[] = L"ServerExecutorChildProcess";
	PROCESS_INFORMATION piProcInfo;
	STARTUPINFOW siStartInfo;
	BOOL bSuccess = FALSE;

	// Set up members of the PROCESS_INFORMATION structure. 

	ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

	// Set up members of the STARTUPINFO structure. 
	// This structure specifies the STDIN and STDOUT handles for redirection.

	ZeroMemory(&siStartInfo, sizeof(STARTUPINFOW));
	siStartInfo.cb = sizeof(STARTUPINFOW);
	siStartInfo.hStdError = m_child_out_write;
	siStartInfo.hStdOutput = m_child_out_write;
	siStartInfo.hStdInput = m_child_in_read;
	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

	// Create the child process. 

	bSuccess = CreateProcessW(NULL,
		szCmdline,     // command line 
		NULL,          // process security attributes 
		NULL,          // primary thread security attributes 
		TRUE,          // handles are inherited 
		0,             // creation flags 
		NULL,          // use parent's environment 
		NULL,          // use parent's current directory 
		&siStartInfo,  // STARTUPINFO pointer 
		&piProcInfo);  // receives PROCESS_INFORMATION 

	 // If an error occurs, exit the application. 
	if (!bSuccess)
	{
		is_created = false;
	}
	else
	{
		// Close handles to the child process and its primary thread.
		// Some applications might keep these handles to monitor the status
		// of the child process, for example. 

		CloseHandle(piProcInfo.hProcess);
		CloseHandle(piProcInfo.hThread);
	}

	return is_created;
}

void ServerExecutor::execute(const std::wstring& command)
{
	DWORD dwWritten;
	

	BOOL is_written = WriteFile(
		m_child_in_write, command.data(), command.size(), &dwWritten, NULL);

	// Close the pipe handle so the child process stops reading. 

	if (!CloseHandle(m_child_in_write))
		std::cerr << "StdInWr CloseHandle Error";
}

std::wstring ServerExecutor::getResult() const
{
	std::wstring result;

	DWORD dwRead, dwWritten;
	CHAR chBuf[BUFSIZE];
	BOOL bSuccess = FALSE;

	CloseHandle(m_child_out_write);

	wchar_t* w_line = nullptr;
	for (;;)
	{
		bSuccess = ReadFile(m_child_out_read, chBuf, BUFSIZE, &dwRead, nullptr);
		if (!bSuccess || dwRead == 0) break;

		w_line = ANSItoUNICODE(chBuf);
		result.append(w_line);

		LocalFree(w_line);
		if (!bSuccess) break;
	}

	return result;
}
