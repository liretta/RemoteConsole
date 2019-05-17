#include "server_router.h"
#include <iostream>

std::wstring ServerRouter::process(const std::wstring& input)
{
	std::wstring result;

	if (!input.empty())
	{
		Marshaller::ModeIndex mode = Marshaller::getMode(input);
		switch (mode)
		{
		case Marshaller::ModeIndex::Authorization:
			{
				if (!m_authorizer)
				{
					initialize_authorizer();
				}
				result = process_authorization(input);
				break;
			}
		case Marshaller::ModeIndex::Command:
			{
				if (!m_executor)
				{
					initialize_executor();
				}
				result = process_execution(input);
				break;
			}
		case Marshaller::ModeIndex::Error:
			{
				process_error_message(input);
				break;
			}
		default:
			std::wcerr << L"ERROR: cannot unpack " << input << std::endl;
		}
	}
	return result;
}

void ServerRouter::process_error_message(const std::wstring& input)
{
	auto text =
		Marshaller::unpackMessage(Marshaller::ModeIndex::Error, input);
	std::wcerr << text << std::endl;
}

std::wstring ServerRouter::process_authorization(const std::wstring& input)
{
	auto auth_data		= Marshaller::unpackAuthorizationData(input);
	bool is_authorized	= m_authorizer->authorize(auth_data);

	return Marshaller::packResult(is_authorized);
}

std::wstring ServerRouter::process_execution(const std::wstring& input)
{
	std::wstring result;

	if (m_executor->isInitialized())
	{
		auto command =
			Marshaller::unpackMessage(Marshaller::ModeIndex::Command, input);

		bool is_executed	= m_executor->execute(command);
		auto output			= m_executor->getResult();

		Marshaller::ModeIndex mode =
			(is_executed ?	Marshaller::ModeIndex::Command :
							Marshaller::ModeIndex::Error);

		result = Marshaller::packMessage(mode, output);
	}
	else
	{
		result =
			Marshaller::packMessage(Marshaller::ModeIndex::Error,
									L"Server is not able to execute commands");
			
		std::cerr << "ERROR: cannot initialize executor" << std::endl;
	}

	return result;
}

void ServerRouter::initialize_authorizer()
{
	m_authorizer = std::make_unique<Authorizer>();
}

void ServerRouter::initialize_executor()
{
	m_executor = std::make_unique<ServerExecutor>();
	m_executor->initialize();
}
