#include "HierBasisFunction1D_ExtraplolateBC.h"
#include "HatFunction.h"
#include "HierBasisFunction1D_HomogenousBC.h"
#include "Pow2.h"

HierBasisFunction1D_ExtraplolateBC::HierBasisFunction1D_ExtraplolateBC(HierLevelIndex li) : m_levelIndex(li)
{
}

HierLevelIndex HierBasisFunction1D_ExtraplolateBC::getLevelIndex() const
{
    return m_levelIndex;
}

double HierBasisFunction1D_ExtraplolateBC::operator()(double x) const
{
    if (x < 0.0 || x > 1.0)
        return 0.0;

    const auto level = m_levelIndex.getLevel();
    if (level == 1)
        return 1.0;

    const auto index = m_levelIndex.getIndex();
    const auto pow2Level = Pow2()(level);
    if (index == 1)
    {
        if (x > 2.0 / pow2Level)
            return 0.0;
        return 2.0 - pow2Level * x;
    }

    if (index == pow2Level - 1)
    {
        if (x < 1 - 2.0 / pow2Level)
            return 0.0;
        return pow2Level * x + 1 - index;
    }

    return HierBasisFunction1D_HomogenousBC::Evaluate(m_levelIndex, x);
}

Interval<double> HierBasisFunction1D_ExtraplolateBC::getSupport() const
{
    if (m_levelIndex.getLevel() == 1)
        return Interval<double>(0, 1);
    return HierBasisFunction1D_HomogenousBC::GetSupport(m_levelIndex);
}

std::unique_ptr<HierBasisFunction1D_ExtraplolateBC> HierBasisFunction1D_ExtraplolateBC_Factory::create(
    const HierLevelIndex &li) const
{
    return std::make_unique<HierBasisFunction1D_ExtraplolateBC>(li);
}

std::vector<HierLevelIndex> HierBasisFunction1D_ExtraplolateBC_Factory::getLowestLevel() const
{
    return std::vector<HierLevelIndex>{{1, 1}};
}

bool HierBasisFunction1D_ExtraplolateBC_Factory::canBeRefined(const HierLevelIndex &) const
{
    return true;
}

const IHierBasisFunction1D *HierBasisFunction1D_ExtraplolateBC_Factory::get(const HierLevelIndex &li)
{
    if (!m_basisFunctions.contains(li))
    {
        m_basisFunctions.emplace(li, create(li));
    }
    return m_basisFunctions.at(li).get();
}