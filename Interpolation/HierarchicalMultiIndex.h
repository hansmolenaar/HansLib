#pragma once

#include <vector>

class HierarchicalMultiIndex
{
public:
   HierarchicalMultiIndex(std::vector<size_t>&& level, std::vector<size_t>&& index);
   size_t getDimension() const;
   const std::vector<size_t> getLevel() const;
   const std::vector<size_t> getIndex() const;

private:
   std::vector<size_t> m_level;
   std::vector<size_t> m_index;
};
