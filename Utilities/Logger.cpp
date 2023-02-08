#include "Logger.h"
#include <numeric>

void Logger::LogLine(const std::string& line)
{
   m_lines.push_back(line);
}

void Logger::LogLine(const std::vector<std::string>& lines)
{
   std::string singleLine;
   for (const auto& line : lines) singleLine += (line + "\n");
   LogLine(singleLine);
}