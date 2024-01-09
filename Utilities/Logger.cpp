#include "Logger.h"

#include <numeric>
#include <chrono>
#include <format>
#include <iostream>

namespace
{
   std::string GetDateTime()
   {
      const auto now = std::chrono::system_clock::now();
      std::ostringstream oss;
      oss << std::format("{:%Y-%m-%d %H:%M:%OS}", now);
      return oss.str();
   }

   std::string GetHeaderLine()
   {
      return GetDateTime();
   }
}

void Logger::logLine(const std::string& line)
{
   logLines(std::vector<std::string>{line});
}

void Logger::logLines(const std::vector<std::string>& lines)
{
   std::ostringstream oss;
   oss << GetHeaderLine() << '\n';

   for (const auto& line : lines)
   {
      oss << line << '\n';
   }
   m_lines.push_back(oss.str());
}

void Logger::logStatictics(const std::string& header, std::span<const double> values)
{

}

const std::vector<std::string>& Logger::get() const
{
   return m_lines;
}