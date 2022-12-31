#pragma once


#include <functional>
#include <initializer_list>
#include <string>
#include <filesystem>
#include <fstream>

namespace Plotting
{
   std::filesystem::path GenerateFullFilePath(const std::string& name, std::string folderName = "C:\\\\Users\\Hans\\Documents\\tmp");
   std::ofstream GetFile(const std::string& name,  std::string folderName = "C:\\\\Users\\Hans\\Documents\\tmp");
   std::string PlotFunction(std::initializer_list< std::function<double(double)> > functions, double xmin, double xmax, int  nsteps);
};
