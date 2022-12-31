#include "HierBasisFunction1D_ExtendedLevelOneBC.h"
#include "Functions/HatFunction.h"
#include "Utilities/Pow2.h"
#include "HierBasisFunction1D_HomogenousBC.h"

#include <map>
#include <memory>

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
   return HierBasisFunction1D_HomogenousBC::GetSupport(m_levelIndex);
}

std::unique_ptr<HierBasisFunction1D_ExtendedLevelOneBC> HierBasisFunction1D_ExtendedLevelOneBC_Factory::create(const HierLevelIndex& li) const
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


const IHierBasisFunction1D* HierBasisFunction1D_ExtendedLevelOneBC_Factory::get(const HierLevelIndex& li) const
{
   static std::map<HierLevelIndex, std::unique_ptr<HierBasisFunction1D_ExtendedLevelOneBC>> s_basisFuncions;
   if (!s_basisFuncions.contains(li))
   {
      s_basisFuncions.emplace(li, create(li));
   }
   return s_basisFuncions.at(li).get();
}
