#include "HierMultiIndex.h"
#include "HierarchicalBasisFunction1D.h"
#include "Utilities/Assert.h"

#include "Utilities/Defines.h"
#include <numeric>

HierMultiIndex::HierMultiIndex(std::vector<HierLevelIndex>&& levelsIndices) :
   m_levelsIndices(std::move(levelsIndices))
{
   Utilities::Assert(!m_levelsIndices.empty());
}

HierMultiIndex::HierMultiIndex(HierLevelIndex levelIndex) :
   m_levelsIndices(std::vector<HierLevelIndex>{levelIndex})
{
}

size_t HierMultiIndex::getDimension() const
{
   return m_levelsIndices.size();
}

const std::vector<HierLevelIndex>& HierMultiIndex::get() const
{
   return m_levelsIndices;
}

size_t HierMultiIndex::getL1NormLevel() const
{
   return std::accumulate(m_levelsIndices.begin(), m_levelsIndices.end(), static_cast<size_t>(0), [](size_t acc, const HierLevelIndex& add) {return acc + add.getLevel(); });
}

size_t HierMultiIndex::getSupNormLevel() const
{
   return str::max(m_levelsIndices | stv::transform([](const HierLevelIndex& li) {return li.getLevel(); }));
}