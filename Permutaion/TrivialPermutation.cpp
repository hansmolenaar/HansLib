#include "TrivialPermutation.h"

TrivialPermutation::TrivialPermutation(int cardinality) : 
   m_cardinality(cardinality),
   m_bounds(BoundsCheck<int>::Create(0,cardinality -1))
{
   Utilities::Assert(cardinality > 0);
}

int TrivialPermutation::operator()(int n) const
{
   m_bounds.check(n);
   return n;
}

int TrivialPermutation::getCardinality() const
{
   return m_cardinality;
}