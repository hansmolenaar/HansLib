
#include "HLUtils/MessageHandler.h"

struct MyException : std::exception {
    MyException(const std::string msg)
        : m_message(msg)
    {
    }
    const std::string m_message;
    const char* what() const noexcept { return m_message.c_str(); }
};

void MessageHandler::Error(const std::string& msg)
{
    throw MyException(msg);
}

void MessageHandler::Assert(bool test, std::string msg)
{
	if (!test)
	{
		if (!msg.empty())
		{
			Error(msg);
		}
		else
		{
			Error("Assertion error");
		}
	}
}