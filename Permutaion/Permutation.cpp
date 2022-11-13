#include "Permutation.h"
#include "Utilities/Assert.h"

Permutation::Permutation(std::vector<int>&& permut) :
   m_permut(std::move(permut))
{
}

 Permutation Permutation::CreateTrivial(int cardinality)
{
   Utilities::Assert(cardinality > 0);
   std::vector<int> permut(cardinality);
   for (int n = 0; n < cardinality; ++n) permut[n] = n;
   return Permutation(std::move( permut));
}

int Permutation::operator()(int n) const
{
   return m_permut.at(n);
}

int Permutation::getCardinality() const
{
   return static_cast<int>(m_permut.size());
}