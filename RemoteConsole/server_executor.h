#pragma once

#include <windows.h> 
#include <tchar.h>
#include <iostream>
#include <strsafe.h>
#include <string>

#define BUFSIZE 4096




class ServerExecutor
{
public:

	ServerExecutor() = default;

	bool initialize();
	bool startProcess();
	void execute(const std::wstring& command);
	std::wstring getResult() const;

private:

	SECURITY_ATTRIBUTES
		m_security_attributes;

	HANDLE
		m_child_out_read,
		m_child_out_write,
		m_child_in_read,
		m_child_in_write;
	
};
