#pragma once

#include <exception>
#include <string>

struct MyException : std::exception {
   MyException(const std::string msg)
      : m_message(msg)
   {
   }
   const std::string m_message;
   const char* what() const noexcept { return m_message.c_str(); }
};


#define ASSERT_MYEXCEPTION_MESSAGE(code, expected_message) do { \
    try {                                                   \
      { code; }                                             \
      FAIL() << "no exception occured" << std::endl;             \
    }                                                       \
    catch (const MyException &e) {                   \
      ASSERT_EQ(e.m_message, expected_message);   \
    }                                                       \
    catch (const std::exception &e) {                       \
      FAIL() << "an unexpected exception occured: " << e.what() << std::endl; \
    }                                                       \
    catch (...) {                                           \
      FAIL() << "an unknown exception occured" << std::endl;     \
    }                                                       \
  } while(0);
