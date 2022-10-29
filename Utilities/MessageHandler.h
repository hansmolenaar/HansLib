#pragma once
#include <string>

class MessageHandler
{
public:
	static void Error(const std::string& msg);
	static void Assert(bool test, std::string msg = "");
};
