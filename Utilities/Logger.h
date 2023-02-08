#pragma once

#include <string>
#include <vector>

class Logger
{
public:
   void LogLine(const std::string&);
   void LogLine(const std::vector<std::string>&);
private:
   std::vector<std::string> m_lines;
};
