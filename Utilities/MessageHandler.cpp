
#include "Utilities/MessageHandler.h"


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