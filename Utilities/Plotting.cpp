#include "Utilities/Plotting.h"
#include "Utilities/Assert.h"

#include <vector>
#include <vector>
#include <sstream>

std::string Plotting::PlotFunction(std::initializer_list< std::function<double(double)> > functions, double xmin, double xmax, int  npoints)
{
   Utilities::Assert(npoints > 1);
   Utilities::Assert(xmax > xmin);

   std::vector<std::function<double(double)> > fies(functions.begin(), functions.end());
   std::vector<std::string> headers{"x"};
   for (size_t n = 0; n < fies.size(); ++n)
   {
      headers.push_back("y" + std::to_string(n));
   }

   std::vector<std::vector<double>> values;
   const double delx = (xmax - xmin) / (npoints - 1);
   for (int n = 0; n < npoints; ++n)
   {
      std::vector<double> row;
      double x = std::numeric_limits<double>::infinity();
      if (n == 0)
      {
         x = xmin;
      }
      else if (n == npoints - 1)
      {
         x = xmax;
      }
      else
      {
         x = xmin + delx * n;
      }
      row.push_back(x);

      for (auto& fie : fies)
      {
         const double y = fie(x);
         row.push_back(y);
      }
      values.push_back(row);
   }

   std::ostringstream result;
   ToStream(result, headers, values);
   return result.str();
}

std::filesystem::path Plotting::GenerateFullFilePath(const std::string& name, std::string folderName)
{
   std::filesystem::path path{ folderName };
   const auto fileName = name + ".txt";
   path /= fileName;
   return path;
}

std::ofstream Plotting::GetFile(const std::string& name, std::string folderName)
{
   const auto& fullPath = Plotting::GenerateFullFilePath(name);
   std::ofstream ofs(fullPath);
   if (!ofs.is_open()) throw MyException(std::string("Unable to create file ") + fullPath.string());
   return ofs;
}

void Plotting::ToStream(std::ostream& stream, const std::vector<std::string>& headers, const std::vector<std::vector<double>>& values)
{
   bool isFirst = true;
   for (const auto& header : headers)
   {
      if (!isFirst)
      {
         stream << " , ";
      }
      else 
      {
         isFirst = false;
      }
      stream << header;
   }
   stream << "\n";

   const auto numColumns = headers.size();
   for (const auto& row : values)
   {
      Utilities::Assert(row.size() == numColumns);
      isFirst = true;
      for (double val : row)
      {
         if (!isFirst)
         {
            stream << " , ";
         }
         else
         {
            isFirst = false;
         }
         stream << val;
      }
      stream << "\n";
   }
}