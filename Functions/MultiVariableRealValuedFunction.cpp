#include "MultiVariableRealValuedFunction.h"
#include "MyAssert.h"
#include "IMatrix.h"
#include "MatrixDense.h"
#include "IMatrixUtils.h"
#include "IRealFunctionUtils.h"


MultiVariableRealValuedFunction::MultiVariableRealValuedFunction(
   std::vector<bool>  isactive,
   std::function<double(std::span<const double>)> function,
   std::function<void(std::span<const double>, std::span< double>)> derivative) :
   m_isActive(isactive), m_function(function), m_derivative(derivative)

{
   Utilities::MyAssert(m_isActive.size() > 0);
}

MultiVariableRealValuedFunction MultiVariableRealValuedFunction::CreateNull(int numVar)
{
   Utilities::MyAssert(numVar > 0);
   auto active = std::vector<bool>(numVar, false);
   return MultiVariableRealValuedFunction(
      active,
      [](std::span<const double>) {return 0.0; },
      [](std::span<const double>, std::span< double>jac) {std::fill(jac.begin(), jac.end(), 0.0); });
}

std::shared_ptr<IMultiVariableRealValuedFunction>  MultiVariableRealValuedFunction::CreateNullShared(int numVar)
{
   auto nul = new MultiVariableRealValuedFunction(MultiVariableRealValuedFunction::CreateNull(numVar));
   return std::shared_ptr<IMultiVariableRealValuedFunction>(nul);
}

int MultiVariableRealValuedFunction::GetDomainDimension() const
{
   return static_cast<int>(m_isActive.size());
}


bool MultiVariableRealValuedFunction::DerivativeAlwaysZero(int var) const
{
   return !m_isActive.at(var);
}


bool MultiVariableRealValuedFunction::HasDerivative() const
{
   return true;
}

double MultiVariableRealValuedFunction::Evaluate(std::span<const double>x) const
{
   Utilities::MyAssert(x.size() == GetDomainDimension());
   return m_function(x);
}

void MultiVariableRealValuedFunction::Derivative(std::span<const double> x, std::span< double> dfdx) const
{
   const int dim = GetDomainDimension();
   Utilities::MyAssert(x.size() == dim);
   Utilities::MyAssert(dfdx.size() == dim);
   std::vector<double> deriv(dim);
   m_derivative(x, deriv);
   for (int n = 0; n < dim; ++n)
   {
      dfdx[n] = deriv[n];
   }

}



MultiVariableRealValuedFunction operator-(const IMultiVariableRealValuedFunction& f, const IMultiVariableRealValuedFunction& g)
{
   Utilities::MyAssert(f.GetDomainDimension() == g.GetDomainDimension());
   const int dim = f.GetDomainDimension();

   std::vector<bool> active(dim);
   for (int n = 0; n < dim; ++n)
   {
      active[n] = !(f.DerivativeAlwaysZero(n) && g.DerivativeAlwaysZero(n));

   }
   std::function<double(std::span<const double>)> eval = [&](std::span<const double> x)
   {
      const double evalF = f.Evaluate(x);
      const double evalG = g.Evaluate(x);
      return evalF - evalG;
   };

   std::function<void(std::span<const double>, std::span< double>)> deriv = [&](std::span<const double> x, std::span< double> df)
   {
      Utilities::MyAssert(x.size() == df.size());
      const auto dim = x.size();

      std::vector<double> dg(dim);
      f.Derivative(x, df);
      g.Derivative(x, dg);

      for (size_t n = 0; n < dim; ++n)
      {
         df[n] -= dg[n];
      }
   };

   return  MultiVariableRealValuedFunction(active, eval, deriv);
}

MultiVariableRealValuedFunction operator/(const IMultiVariableRealValuedFunction& numer, const IMultiVariableRealValuedFunction& denom)
{
   Utilities::MyAssert(numer.GetDomainDimension() == denom.GetDomainDimension());
   const auto dim = numer.GetDomainDimension();

   std::vector<bool> active(dim);
   for (int n = 0; n < dim; ++n)
   {
      active[n] = !(numer.DerivativeAlwaysZero(n) && denom.DerivativeAlwaysZero(n));

   }
   std::function<double(std::span<const double>)> eval = [&](std::span<const double> x)
   {
      double n = Evaluate(numer, x);
      double d = Evaluate(denom, x);
      return n / d;
   };

   std::function<void(std::span<const double>, std::span< double>)> deriv = [&](std::span<const double> x, std::span< double> df)
   {
      Utilities::MyAssert(x.size() == df.size());
      const auto dim = x.size();

      const double evalNumer = Evaluate(numer, x);
      const double evalDenom = Evaluate(denom, x);

      std::vector<double> dnumer(dim);
      std::vector<double> ddenom(dim);
      Derivative(numer, x, dnumer);
      Derivative(denom, x, ddenom);

      const double d = 1 / (evalDenom * evalDenom);
      for (size_t n = 0; n < dim; ++n)
      {
         df[n] = (dnumer[n] * evalDenom - evalNumer * ddenom[n]) * d;
      }
   };

   return  MultiVariableRealValuedFunction(active, eval, deriv);
}

