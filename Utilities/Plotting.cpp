#include "Utilities/Plotting.h"
#include "Utilities/Assert.h"

#include <vector>
#include <vector>
#include <sstream>

std::string Plotting::PlotFunction(std::initializer_list< std::function<double(double)> > functions, double xmin, double xmax, int  nsteps)
{
   Utilities::Assert(nsteps > 1);
   Utilities::Assert(xmax > xmin);
   std::ostringstream result;

   std::vector<std::function<double(double)> > fies(functions.begin(), functions.end());
   const double delx = (xmax - xmin) / (nsteps - 1);
   for (int n = 0; n < nsteps; ++n)
   {
      double x = std::numeric_limits<double>::infinity();
      if (n == 0)
      {
         x = xmin;
      }
      else if (n == nsteps - 1)
      {
         x = xmax;
      }
      else
      {
         x = xmin + delx * n;
      }


      result << x;
      for (auto& fie : fies)
      {
         const double y = fie(x);
         result << " , " << y;
      }
      result << std::endl;
   }
   const std::string str = result.str();
   return str;
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