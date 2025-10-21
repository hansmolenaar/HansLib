#include "HierLevelIndex.h"
#include "MyAssert.h"
#include "MyException.h"
#include "Pow2.h"

#include <sstream>

namespace
{

void CheckLevelIndex(const HierLevelIndex &li)
{
    const auto level = li.getLevel();
    const auto index = li.getIndex();
    if (level == 0)
    {
        if (index > 1)
        {
            std::ostringstream os;
            os << "HierarchicalMultiIndex CheckIndex at level=0 ??: " << li;
            throw MyException(os.str());
        }
        return;
    }

    if (index > Pow2()(level))
    {
        std::ostringstream os;
        os << "HierarchicalMultiIndex CheckIndex index out of bounds: " << li;
        os << "HierarchicalMultiIndex CheckIndex index out of bounds: " << li;
        throw MyException(os.str());
    }
    if (index % 2 == 0)
    {
        std::ostringstream os;
        os << "HierarchicalMultiIndex CheckIndex index should be odd, actual=" << li;
        throw MyException(os.str());
    }
}

HierLevelIndex Reduce(size_t level, size_t index)
{
    while (level > 0 && index % 2 == 0)
    {
        level -= 1;
        index /= 2;
    }
    return HierLevelIndex(level, index);
}

} // namespace

HierLevelIndex::HierLevelIndex(size_t level, size_t index) : m_level(level), m_index(index)
{
    CheckLevelIndex(*this);
}

size_t HierLevelIndex::getLevel() const
{
    return m_level;
}

size_t HierLevelIndex::getIndex() const
{
    return m_index;
}

double HierLevelIndex::toDouble() const
{
    return static_cast<double>(getIndex()) / Pow2()(getLevel());
}

std::array<HierLevelIndex, 2> HierLevelIndex::refine() const
{
    return {HierLevelIndex(getLevel() + 1, 2 * getIndex() - 1), HierLevelIndex(getLevel() + 1, 2 * getIndex() + 1)};
}