#pragma once
#include "HierarchicalLevelIndex.h"
#include <cstddef>

class  HierarchicalBasisFunction1D
{
public:

   explicit HierarchicalBasisFunction1D(HierarchicalLevelIndex li);
   double operator()(double x) const;

private:
   HierarchicalLevelIndex m_levelIndex;
};
