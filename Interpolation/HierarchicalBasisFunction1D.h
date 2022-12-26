#pragma once

#include <cstddef>

class  HierarchicalBasisFunction1D
{
public:

   HierarchicalBasisFunction1D(size_t level, size_t index);
   double evaluate(double x) const;
   static void CheckLevelIndex(size_t level, size_t index);
private:
   size_t m_level;
   size_t m_index;
};
