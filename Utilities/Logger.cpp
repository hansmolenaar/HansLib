#include "ArrayStatistics.h"
#include "Logger.h"

#include <chrono>
#include <format>
#include <fstream>
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

void Logger::logLine(const std::ostringstream& os)
{
   logLine(os.str());
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

const std::vector<std::string>& Logger::get() const
{
   return m_lines;
}

void Logger::toFile(std::filesystem::path filePath)const
{
   std::ofstream stream(filePath.string().c_str(), std::ios::out);
   for (const auto& line : m_lines)
   {
      stream << line;
   }
   stream.close();
}