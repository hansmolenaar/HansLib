#include "HierarchicalMultiIndex.h"
#include "HierarchicalBasisFunction1D.h"
#include "Utilities/Assert.h"

#include "Utilities/Defines.h"
#include <numeric>

HierarchicalMultiIndex::HierarchicalMultiIndex(std::vector<HierarchicalLevelIndex>&& levelsIndices) :
   m_levelsIndices(std::move(levelsIndices))
{
   Utilities::Assert(!m_levelsIndices.empty());
}

HierarchicalMultiIndex::HierarchicalMultiIndex(HierarchicalLevelIndex levelIndex) : 
   m_levelsIndices(std::vector<HierarchicalLevelIndex>{levelIndex})
{
}

size_t HierarchicalMultiIndex::getDimension() const
{
   return m_levelsIndices.size();
}

const std::vector<HierarchicalLevelIndex>& HierarchicalMultiIndex::get() const
{
   return m_levelsIndices;
}

size_t HierarchicalMultiIndex::getL1NormLevel() const
{
   return std::accumulate(m_levelsIndices.begin(), m_levelsIndices.end(), static_cast<size_t>(0), [](size_t acc, const HierarchicalLevelIndex& add) {return acc + add.getLevel(); });
}

size_t HierarchicalMultiIndex::getSupNormLevel() const
{
   auto tmp1 = m_levelsIndices | std::views::transform([](const HierarchicalLevelIndex& li) {return li.getLevel(); });
   auto tmp2 = std::ranges::max(tmp1);
   return std::ranges::max(m_levelsIndices | std::views::transform([](const HierarchicalLevelIndex& li) {return li.getLevel(); }));
}