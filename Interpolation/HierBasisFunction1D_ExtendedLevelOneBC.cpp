#include "HierBasisFunction1D_ExtendedLevelOneBC.h"
#include "Functions/HatFunction.h"
#include "Utilities/Pow2.h"

HierBasisFunction1D_ExtendedLevelOneBC::HierBasisFunction1D_ExtendedLevelOneBC(HierLevelIndex hli) : m_levelIndex(hli)
{
}

HierLevelIndex HierBasisFunction1D_ExtendedLevelOneBC::getLevelIndex() const
{
   return m_levelIndex;
}

double HierBasisFunction1D_ExtendedLevelOneBC::operator()(double x) const
{
   return HatFunction()(Pow2()(m_levelIndex.getLevel()) * x - m_levelIndex.getIndex());
}


Interval<double> HierBasisFunction1D_ExtendedLevelOneBC::getSupport() const
{
   if (m_levelIndex.getLevel() == 0) return Interval<double>(0, 1);
   const double invpow2 = 1.0 / Pow2()(m_levelIndex.getLevel());
   return Interval<double>(invpow2 * (m_levelIndex.getIndex() - 1), invpow2 * (m_levelIndex.getIndex() + 1));
}

std::unique_ptr<IHierBasisFunction1D> HierBasisFunction1D_ExtendedLevelOneBC_Factory::create(const HierLevelIndex& li) const
{
   return std::make_unique<HierBasisFunction1D_ExtendedLevelOneBC>(li); 
}

std::vector<HierLevelIndex> HierBasisFunction1D_ExtendedLevelOneBC_Factory::getLowestLevel() const
{
   return std::vector<HierLevelIndex>{HierLevelIndex(0, 0), HierLevelIndex(0, 1), HierLevelIndex(1, 1)};
}

bool HierBasisFunction1D_ExtendedLevelOneBC_Factory::canBeRefined(const HierLevelIndex& hli) const
{
   return hli.getLevel() > 0;
}
