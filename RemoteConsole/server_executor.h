#pragma once

#include <windows.h> 
#include <tchar.h>
#include <iostream>
#include <strsafe.h>
#include <string>

#define BUFSIZE 4096




static wchar_t* ANSItoUNICODE(char* line)
{
    std::size_t
		length		= strlen(line) + 1,
		size_needed = MultiByteToWideChar(CP_OEMCP, 0, line, length, NULL, 0);

	wchar_t* w_line = static_cast<wchar_t*>(LocalAlloc(LPTR, sizeof(wchar_t) * size_needed));

    MultiByteToWideChar(CP_OEMCP, 0, line, length, w_line, size_needed);
    return w_line;
}

class ServerExecutor
{
public:

	bool initialize()
	{
		m_security_attributes.nLength				= sizeof(SECURITY_ATTRIBUTES);
		m_security_attributes.bInheritHandle		= TRUE;
		m_security_attributes.lpSecurityDescriptor	= nullptr;

		bool is_initialized = true;

		if (!CreatePipe(&m_child_out_read, &m_child_out_write, &m_security_attributes, 0))
		{
			is_initialized = false;
		}

		// Ensure the read handle to the pipe for STDOUT is not inherited.
		if (!SetHandleInformation(m_child_out_read, HANDLE_FLAG_INHERIT, 0))
		{
			is_initialized = false;
		}

		return  is_initialized;
	}

	void execute(const std::wstring& command)
	{
		wchar_t w_command[MAX_PATH];
		wcscpy_s(w_command, command.data());

		PROCESS_INFORMATION process_information;
		ZeroMemory(&process_information, sizeof(PROCESS_INFORMATION));

		STARTUPINFOW startup_info;
		// Set up members of the STARTUPINFO structure. 
		// This structure specifies the STDIN and STDOUT handles for redirection.
		ZeroMemory(&startup_info, sizeof(STARTUPINFOW));
		startup_info.cb				= sizeof(STARTUPINFOW);
		startup_info.hStdError		= m_child_out_write;
		startup_info.hStdOutput		= m_child_out_write;
		startup_info.hStdInput		= nullptr;
		startup_info.dwFlags		|= STARTF_USESTDHANDLES;

		// Create the child process. 
		BOOL is_created = FALSE;
		is_created = CreateProcessW(
			NULL,
			w_command,				// command line 
			NULL,					// process security attributes 
			NULL,					// primary thread security attributes 
			TRUE,					// handles are inherited 
			0,						// creation flags 
			NULL,					// use parent's environment 
			NULL,					// use parent's current directory 
			&startup_info,			// STARTUPINFOW pointer 
			&process_information	// receives PROCESS_INFORMATION 
		);  

		// If an error occurs, exit the application. 
		if (!is_created)
		{
			//
		}
		else
		{
			// Close handles to the child process and its primary thread.
			// Some applications might keep these handles to monitor the status
			// of the child process, for example. 

			CloseHandle(process_information.hProcess);
			CloseHandle(process_information.hThread);
		}
	}

	std::wstring getResult() const
	{
		std::wstring result;

		DWORD read_count = 0;
		char buffer[BUFSIZE];
		BOOL is_read = FALSE;
		HANDLE h_parent_out = GetStdHandle(STD_OUTPUT_HANDLE);


		wchar_t* w_tmp_buffer = nullptr;
		do
		{
			is_read = ReadFile(m_child_out_read, buffer, BUFSIZE, &read_count, NULL);

			w_tmp_buffer = ANSItoUNICODE(buffer);
			result.append(w_tmp_buffer);

			LocalFree(w_tmp_buffer);
		} while (is_read && read_count != 0);


		return result;
	}

private:

	SECURITY_ATTRIBUTES
		m_security_attributes;

	HANDLE
		m_child_out_read,
		m_child_out_write;
	
};
