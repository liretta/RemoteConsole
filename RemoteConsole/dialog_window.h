#pragma once
#include <string>
#include <iostream>

class IDialogWindow
{
public:
	virtual std::string input() = 0;
	virtual void output(const std::string &str) = 0;
};

class DialogWindowConsole: public IDialogWindow
{
public:
	static std::string input()
	{
		std::string tmp_str;
		getline(std::cin, tmp_str);
		return tmp_str;
	};

	static void output(const std::string &str)
	{
		std::cout << str;
	};
};

class DialogWIndowConcoleAuth: public IDialogWindow
{
public:
	static std::pair<std::string, std::string> input()
	{
		std::string login, pass;
		std::cin >> login;
		std::cin >> pass;
		return std::make_pair(login, pass);
	}

	static void output(const std::string &str)
	{
		std::cout << str;
	}
};
