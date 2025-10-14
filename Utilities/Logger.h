#pragma once

#include <filesystem>
#include <span>
#include <sstream>
#include <string>
#include <vector>

class Logger
{
public:
   void logLine(const std::ostringstream& os);
   void logLine(const std::string&);
   void logLines(const std::vector<std::string>&);
   const std::vector<std::string>& get() const;
   void toFile(std::filesystem::path filePath)const;
private:
   std::vector<std::string> m_lines;
};
