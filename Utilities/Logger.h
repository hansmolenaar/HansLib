#pragma once

#include <string>
#include <vector>
#include <span>
class Logger
{
public:
   void logLine(const std::string&);
   void logLines (const std::vector<std::string>&);
   const std::vector<std::string>& get() const;
private:
   std::vector<std::string> m_lines;
};
