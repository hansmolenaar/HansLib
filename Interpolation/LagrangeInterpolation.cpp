#include "LagrangeInterpolation.h"
#include <exception>


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
   throw std::exception(" LagrangeInterpolation::Evaluate not implemented");
}

double LagrangeInterpolation::Derivative(double x) const
{
   throw std::exception(" LagrangeInterpolation::Derivative not implemented");
}

bool LagrangeInterpolation::IsNonConstant() const
{
   return true;
}