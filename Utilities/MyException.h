#pragma once
#include <string>

struct MyException : std::exception {
   MyException(const std::string msg)
      : m_message(msg)
   {
   }
   const std::string m_message;
   const char* what() const noexcept { return m_message.c_str(); }
};
