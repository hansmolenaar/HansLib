#include "HierBasisFunction1D_HomogenousBC.h"
#include "Functions/HatFunction.h"
#include "Utilities/Assert.h"
#include "Utilities/Pow2.h"

HierBasisFunction1D_HomogenousBC::HierBasisFunction1D_HomogenousBC(HierLevelIndex levelIndex) :
   m_levelIndex(levelIndex)
{
   Utilities::Assert(m_levelIndex.getLevel() > 0);
}

double HierBasisFunction1D_HomogenousBC::operator()(double x) const
{
   return Evaluate(m_levelIndex, x);
}

HierLevelIndex HierBasisFunction1D_HomogenousBC::getLevelIndex() const
{
   return m_levelIndex;
}

Interval<double> HierBasisFunction1D_HomogenousBC::getSupport() const
{
   return GetSupport(m_levelIndex);
}

double HierBasisFunction1D_HomogenousBC::Evaluate(const HierLevelIndex& levelIndex, double x)
{
   return HatFunction()(Pow2()(levelIndex.getLevel()) * x - levelIndex.getIndex());
}

Interval<double> HierBasisFunction1D_HomogenousBC::GetSupport(const HierLevelIndex& levelIndex)
{
   const double invpow2 = 1.0 / Pow2()(levelIndex.getLevel());
   return Interval<double>(invpow2 * (levelIndex.getIndex() - 1), invpow2 * (levelIndex.getIndex() + 1));
}