#pragma once

#include <windows.h> 
#include <string>

#define BUFSIZE 4096


class IServerExecutor
{
public:
	virtual ~IServerExecutor() = default;

	
	virtual bool			initialize()							= 0;
	virtual void			execute(const std::wstring& command)	= 0;
	virtual std::wstring	getResult() const						= 0;
};

class ServerExecutorStub : public IServerExecutor
{
	//
};



class ServerExecutor : public IServerExecutor
{
public:

	ServerExecutor()			= default;
	virtual ~ServerExecutor()	= default;

	/*!
	 * Set up security attributes and create a pipe.
	 * @return initialization status.
	 */
	bool			initialize()							override;

	/*!
	 * Execute Unicode command in a new sub-process.
	 */
	void			execute(const std::wstring& command)	override;

	/*!
	 * Get output from the sub-process and transform it to Unicode.
	 * @return Unicode stdout of the sub-process.
	 */
	std::wstring	getResult() const						override;

	bool isInitialized() const;

private:

	/*!
	 * Write the error message to sub-process output if the error exists.
	 */
	void send_error_message();

	bool create_sub_process(PROCESS_INFORMATION& process_info,
							wchar_t* w_command);

	SECURITY_ATTRIBUTES
		m_security_attributes;

	HANDLE
		m_child_out_read,
		m_child_out_write;

	bool m_is_initialized = false;
};
