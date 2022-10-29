#pragma once


#include <functional>
#include <initializer_list>
#include <string>

namespace Plotting
{

   std::string PlotFunction(std::initializer_list< std::function<double(double)> > functions, double xmin, double xmax, int  nsteps);
};
