#include "HierBasisFunction1D_HomogenousBC.h"
#include "Functions/HatFunction.h"
#include "MyAssert.h"
#include "Pow2.h"

// Factory

const IHierBasisFunction1D* HierBasisFunction1D_HomogenousBC_Factory::get(const HierLevelIndex& li)
{
   if (!m_basisFunctions.contains(li))
   {
      m_basisFunctions.emplace(li, create(li));
   }
   return m_basisFunctions.at(li).get();
}

// Function


HierBasisFunction1D_HomogenousBC::HierBasisFunction1D_HomogenousBC(HierLevelIndex levelIndex) :
   m_levelIndex(levelIndex)
{
   Utilities::MyAssert(m_levelIndex.getLevel() > 0);
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

 std::unique_ptr<HierBasisFunction1D_HomogenousBC> HierBasisFunction1D_HomogenousBC_Factory::create(const HierLevelIndex& li) const
 { 
    return std::make_unique<HierBasisFunction1D_HomogenousBC>(li); 
 }