#include "MultiVariableFunctionEvaluateFrom01.h"
#include "Utilities/Defines.h"

namespace
{
   const Interval<double> c_unit(0, 1);
}

MultiVariableFunctionEvaluateFrom01::MultiVariableFunctionEvaluateFrom01(std::unique_ptr<IMultiVariableFunctionEvaluate>&& fie, std::vector<Interval<double>>&& targetIntervals) :
   m_function(std::move(fie)), m_targetIntervals(std::move(targetIntervals))

{
   if (m_function->getDimension() != m_targetIntervals.size())
   {
      throw MyException("MultiVariableFunctionEvaluateFrom01::MultiVariableFunctionEvaluateFrom01( dimension mismatch");
   }
}

int MultiVariableFunctionEvaluateFrom01::getDimension() const
{
   return m_function->getDimension();
}

double MultiVariableFunctionEvaluateFrom01::operator()(std::span<const double> x) const
{
   if (x.size() != m_targetIntervals.size())
   {
      throw MyException("MultiVariableFunctionEvaluateFrom01::operator() dimension mismatch");
   }

   if (!str::all_of(x, [](double x) {return c_unit.contains(x); }))
   {
      throw MyException("MultiVariableFunctionEvaluateFrom01::operator() argument out of interval [0,1]");
   }

   std::vector<double> xyz(m_targetIntervals.size());
   std::transform(m_targetIntervals.begin(), m_targetIntervals.end(), x.begin(), xyz.begin(),
      [](const auto& interval, double x) {return interval.getLower() + x * interval.getMeasure(); });
   return (*m_function)(xyz);
}