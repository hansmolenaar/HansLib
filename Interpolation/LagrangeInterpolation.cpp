#include "LagrangeInterpolation.h"
#include "Utilities/MessageHandler.h"

LagrangeInterpolation::LagrangeInterpolation(std::span<const double>x, std::span<const double>y) :
   m_xvals(x.begin(), x.end()), m_yvals(y.begin(), y.end())
{
   MessageHandler::Assert(m_xvals.size() == m_yvals.size());
   MessageHandler::Assert(m_xvals.size() > 1);
}

bool  LagrangeInterpolation::HasDerivative() const
{
   return false;
}

bool LagrangeInterpolation::DerivativeAlwaysZero(int eqn, int var) const
{
   return false;
}

double  LagrangeInterpolation::Evaluate(double x) const
{
   double result = 0;
   for (size_t i = 0; i < m_xvals.size(); ++i)
   {
      double coef = 1;
      for (size_t j = 0; j < m_yvals.size(); ++j)
      {
         if (i != j)
         {
            coef *= (x - m_xvals.at(j)) / (m_xvals.at(i) - m_xvals.at(j));
         }
      }
      result += coef * m_yvals.at(i);
   }
   return result;
}

double LagrangeInterpolation::Derivative(double x) const
{
   MessageHandler::Assert(false, " LagrangeInterpolation::Derivative not implemented");
   return {};
}

bool LagrangeInterpolation::IsNonConstant() const
{
   return true;
}