#pragma once

#include "IPermutation.h"
#include "Utilities/BoundsCheck.h"

class TrivialPermutation : public IPermutation
{
public:
   TrivialPermutation(int);
   int operator()(int) const override;
   int getCardinality() const override;
private:
   int m_cardinality;
   BoundsCheck<int> m_bounds;
};