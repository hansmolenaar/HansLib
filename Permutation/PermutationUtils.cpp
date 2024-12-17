#include "PermutationUtils.h"
#include "Defines.h"
#include "MyAssert.h"

bool PermutationUtils::IsPermutation(std::span<const int> permut)
{
   if (permut.empty()) return false;
   std::vector<int> isSet(permut.size(), 0);
   for (const auto& n : permut)
   {
      if (n >= isSet.size()) return false;
      if (!isSet[n]) isSet[n] = 1;
      else return false;
   }
   return str::all_of(isSet, [](int n) { return n != 0; });
}

std::optional<size_t> PermutationUtils::findIdentity(std::span<const Permutation> permutations)
{
   if (permutations.empty()) return {};
   const Permutation identity = Permutation::CreateTrivial(permutations.front().getCardinality());
   const auto found = str::find(permutations, identity);
   if (found == permutations.end()) return {};
   return std::distance(permutations.begin(), found);
}

bool PermutationUtils::areUnique(std::span<const Permutation> permutations)
{
   const size_t numPermutations = permutations.size();
   if (numPermutations < 2) return true;

   for (size_t p = 1; p < numPermutations; ++p)
   {
      const auto findEnd = permutations.begin() + p;
      if (std::find(permutations.begin(), findEnd, permutations[p]) != findEnd) return false;
   }
   return true;
}
