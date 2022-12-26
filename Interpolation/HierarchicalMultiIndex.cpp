#include "HierarchicalMultiIndex.h"
#include "HierarchicalBasisFunction1D.h"
#include "Utilities/Assert.h"

#include "Utilities/Defines.h"
#include <numeric>


HierarchicalMultiIndex::HierarchicalMultiIndex(std::vector<size_t>&& level, std::vector<size_t>&& index) :
   m_level(std::move(level)), m_index(std::move(index))
{
   Utilities::Assert(m_level.size() == m_index.size());
   Utilities::Assert(!m_level.empty());
   for (size_t n = 0; n < m_level.size(); ++n)
   {
      HierarchicalBasisFunction1D::CheckLevelIndex(m_level.at(n), m_index.at(n));
   }
}

size_t HierarchicalMultiIndex::getDimension() const
{
   return m_level.size();
}

const std::vector<size_t> HierarchicalMultiIndex::getLevel() const
{
   return m_level;
}

const std::vector<size_t> HierarchicalMultiIndex::getIndex() const
{
   return m_index;
}

size_t HierarchicalMultiIndex::getL1NormLevel() const
{
   return std::accumulate(m_level.begin(), m_level.end(),static_cast<size_t>(0), std::plus<size_t>() );
}

size_t HierarchicalMultiIndex::getSupNormLevel() const
{
   return *str::max_element(m_level);
}