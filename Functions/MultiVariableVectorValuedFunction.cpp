#include "MultiVariableVectorValuedFunction.h"

#include "IRealFunctionUtils.h"
#include "MyAssert.h"
#include "MatrixSquare.h"



MultiVariableVectorValuedFunction::MultiVariableVectorValuedFunction(std::span< std::shared_ptr<IMultiVariableRealValuedFunction> >  components) :
   m_components(components.begin(), components.end())
{
   const int numVar = m_components[0]->GetDomainDimension();
   for (const auto cmp : m_components)
   {
      Utilities::MyAssert(cmp->GetDomainDimension() == numVar);
   }
}

bool MultiVariableVectorValuedFunction::DerivativeAlwaysZero(int eqn, int var) const
{
   const auto cmp = m_components[eqn];
   return cmp->DerivativeAlwaysZero(0, var);
}


bool MultiVariableVectorValuedFunction::HasDerivative() const
{
   return true;
}

int MultiVariableVectorValuedFunction::GetRangeDimension() const
{
   return static_cast<int>(m_components.size());
}

int MultiVariableVectorValuedFunction::GetDomainDimension() const
{
   return m_components[0]->GetDomainDimension();
}

void MultiVariableVectorValuedFunction::Evaluate(std::span<const double>x, std::span<double> fx)const
{
   int eqn = 0;
   double eval;
   std::span<double> evalWrapper(&eval, 1);
   for (const auto cmp : m_components)
   {
      cmp->Evaluate(x, evalWrapper);
      fx[eqn] = eval;
      ++eqn;
   }
}


void MultiVariableVectorValuedFunction::Derivative(std::span<const double>x, IMatrix& dfdx)const
{
   const int numVar = GetDomainDimension();
   const int numEqn = GetRangeDimension();
   Utilities::MyAssert(dfdx.GetRowDimension() == numEqn);
   Utilities::MyAssert(dfdx.GetColDimension() == numVar);
   std::vector<double> deriv(numVar);
   int eqn = 0;
   for (const auto cmp : m_components)
   {
      ::Derivative(*cmp, x, deriv);
      for (int var = 0; var < numVar; ++var)
      {
         dfdx(eqn, var) = deriv[var];
      }
      ++eqn;
   }
}
