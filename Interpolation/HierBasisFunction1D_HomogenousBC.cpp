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
   return HatFunction()(Pow2()(m_levelIndex.getLevel())*x - m_levelIndex.getIndex());
}