#include "Permutation.h"
#include "Utilities/Assert.h"
#include "PermutationUtils.h"
#include "Utilities/BoundsCheck.h"
#include "Utilities/Defines.h"


Permutation::Permutation(std::vector<int>&& permut) :
   m_permut(permut)
{
}

Permutation Permutation::CreateTrivial(int cardinality)
{
   Utilities::Assert(cardinality > 0);
   std::vector<int> permut(cardinality);
   for (int n = 0; n < cardinality; ++n) permut[n] = n;
   return Permutation(std::move(permut));
}

int Permutation::operator()(int n) const
{
   return m_permut.at(n);
}

int Permutation::getCardinality() const
{
   return static_cast<int>(m_permut.size());
}

Permutation Permutation::Create(std::span<const int> permutSpan)
{
   std::vector<int> permut(permutSpan.begin(), permutSpan.end());
   Utilities::Assert(PermutationUtils::IsPermutation(permut));
   return Permutation(std::move(permut));
}

Permutation Permutation::CreateFromCycle(int cardinality, std::span<const int> cycle)
{
   Utilities::Assert(cardinality > 0);
   const auto bounds = BoundsCheck<int>::Create(0, cardinality - 1);
   Utilities::Assert(str::all_of(cycle, bounds));
   std::vector<int> permut(cardinality, -1);
   for (size_t n = 0; n < cycle.size(); ++n)
   {
      permut.at(cycle[n]) = cycle[(n + 1) % cycle.size()];
   }
   for (size_t n = 0; n < permut.size(); ++n)
   {
      if (permut.at(n) == -1) permut.at(n) = static_cast<int>(n);
   }
   return Permutation::Create(permut);
}
