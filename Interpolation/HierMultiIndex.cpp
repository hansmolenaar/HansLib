#include "HierMultiIndex.h"
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

std::strong_ordering operator<=>(const HierMultiIndex& lhs, const HierMultiIndex& rhs)
{
   auto result = lhs.m_levelsIndices.size() <=> rhs.m_levelsIndices.size();
   if (result != std::strong_ordering::equivalent) return result;

   // First compare levels, then indices
   for (size_t n = 0; n < lhs.m_levelsIndices.size(); ++n)
   {
      result = lhs.m_levelsIndices.at(n).getLevel() <=> rhs.m_levelsIndices.at(n).getLevel();
      if (result != std::strong_ordering::equivalent) return result;
   }

   for (size_t n = 0; n < lhs.m_levelsIndices.size(); ++n)
   {
      result = lhs.m_levelsIndices.at(n).getIndex() <=> rhs.m_levelsIndices.at(n).getIndex();
      if (result != std::strong_ordering::equivalent) return result;
   }

   return std::strong_ordering::equivalent;
}

std::vector<double> HierMultiIndex::toDoubles() const
{
   std::vector<double> result(getDimension());
   str::transform(m_levelsIndices, result.begin(), [](const auto& li) {return li.toDouble(); });
   return result;
}

std::vector<HierMultiIndex> HierMultiIndex::refine(const RefineInDirectionPredicate& predicate) const
{
   std::vector <HierMultiIndex> result;
   for (size_t d = 0; d < m_levelsIndices.size(); ++d)
   {
      if (predicate(*this, d))
      {
         for (const auto& level1D : m_levelsIndices[d].refine())
         {
            auto levels = m_levelsIndices;
            levels[d] = level1D;
            result.emplace_back(std::move(levels));
         }
      }
   }
   return result;
}