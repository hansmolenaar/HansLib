#include "Functions/MultiVariableMonomial.h"
#include "MyAssert.h"


MultiVariableMonomial::MultiVariableMonomial(std::span<const int> powers)
{
   Utilities::MyAssert(powers.size() > 0, "Expect non-empty monomial");
   for (int pow : powers)
   {
      m_powers.push_back(SingleVariableMonomial(pow));
   }
}

MultiVariableMonomial::MultiVariableMonomial(std::initializer_list<int> powers)
{
   Utilities::MyAssert(powers.size() > 0, "Expect non-empty monomial");
   for (int pow : powers)
   {
      m_powers.push_back(SingleVariableMonomial(pow));
   }
}

int MultiVariableMonomial::GetDomainDimension() const
{
   return static_cast<int>(m_powers.size());
}



bool MultiVariableMonomial::HasDerivative() const
{
   return true;
}

bool MultiVariableMonomial::DerivativeAlwaysZero(int var) const
{
   Utilities::MyAssert(var >= 0 && var < m_powers.size());
   return m_powers[var].GetPower() == 0;
}

double MultiVariableMonomial::Evaluate(std::span<const double>x) const
{
   Utilities::MyAssert(x.size() == GetDomainDimension());
   double result = 1;
   int pos = 0;
   for (auto& factor : m_powers)
   {
      result *= factor.Evaluate(x[pos]);
      ++pos;
   }
   return result;
}

void MultiVariableMonomial::Derivative(std::span<const double>x, std::span< double> dfdx)const
{
   const int dim = GetDomainDimension();

   Utilities::MyAssert(x.size() == GetDomainDimension());
   Utilities::MyAssert(dfdx.size() == dim);

   for (int var = 0; var < dim; ++var)
   {
      double result = 1;
      int pos = 0;
      for (auto& factor : m_powers)
      {
         result *= pos != var ? factor.Evaluate(x[pos]) : factor.Derivative(x[pos]);
         ++pos;
      }
      dfdx[var] = result;
   }
}