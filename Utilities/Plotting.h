#pragma once


#include <functional>
#include <initializer_list>
#include <string>
#include <filesystem>
#include <fstream>
#include <span>

namespace Plotting
{
   std::filesystem::path GenerateFullFilePath(const std::string& name, std::string folderName = "C:\\\\Users\\Hans\\Documents\\tmp");
   std::ofstream GetFile(const std::string& name, std::string folderName = "C:\\\\Users\\Hans\\Documents\\tmp");
   std::string PlotFunction(std::initializer_list< std::function<double(double)> > functions, double xmin, double xmax, int  npoints);
   std::vector<std::vector<double>> EvaluateFunctions(std::initializer_list< std::function<double(double)> > functions, double xmin, double xmax, int  npoints);
   std::vector<std::vector<double>> EvaluateFunctionsOnUnit(size_t dim, std::initializer_list< std::function<double(std::vector<double>)> > functions, int  npoints);
   void ToStream(std::ostream& stream, const std::vector<std::vector<double>>& values);
   void ToStream(std::ostream& stream, const std::vector<std::string>& header, const std::vector<std::vector<double>>& values);
   void MdFunctionsOnUnityToStream(std::ostream& stream, size_t dim, std::initializer_list< std::function<double(std::vector<double>)> > functions, int  npoints, std::vector<std::string> headers = std::vector<std::string>{});
   void MdFunctionsOnUnityToFile(const std::string& name, size_t dim, std::initializer_list< std::function<double(std::vector<double>)> > functions, int  npoints, std::vector<std::string> headers = std::vector<std::string>{});
};
