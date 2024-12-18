#include "PermutationUtils.h"
#include "Defines.h"
#include "MyAssert.h"
#include <set>

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
   auto comp = [&permutations](size_t x, size_t y) { return permutations[x] < permutations[y]; };
   auto permSorted = std::set<size_t, decltype(comp)>(comp);
   for (size_t n = 0; n < permutations.size(); ++n)
   {
      if (!permSorted.emplace(n).second) return false;
   }
   return true;
}
