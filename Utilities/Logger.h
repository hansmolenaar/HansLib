#pragma once

#include <string>
#include <vector>
#include <span>
#include <filesystem>

class Logger
{
public:
   void logLine(const std::string&);
   void logLines(const std::vector<std::string>&);
   const std::vector<std::string>& get() const;
   void toFile(std::filesystem::path filePath)const;
private:
   std::vector<std::string> m_lines;
};
