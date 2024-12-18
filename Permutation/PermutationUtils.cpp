#include "PermutationUtils.h"
#include "Defines.h"
#include "MyAssert.h"
#include <set>

bool PermutationUtils::IsPermutation(std::span<const Permutation::Entry> permut)
{
   std::vector<Permutation::Entry> isSet(permut.size(), Permutation::InvalidEntry);
   for (const auto& n : permut)
   {
      if (n >= isSet.size()) return false;
      if (isSet[n] == Permutation::InvalidEntry) isSet[n] = n;
      else return false;
   }
   return str::all_of(isSet, [](Permutation::Entry n) { return n != Permutation::InvalidEntry; });
}

std::optional<Permutation::Entry> PermutationUtils::findIdentity(std::span<const Permutation> permutations)
{
   if (permutations.empty()) return {};
   const auto found = str::find_if(permutations, isIdentity);
   if (found == permutations.end()) return {};
   return static_cast<Permutation::Entry>(std::distance(permutations.begin(), found));
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

bool PermutationUtils::isIdentity(const Permutation& permutation)
{
   for (Permutation::Entry n = 0; n < permutation.getCardinality(); ++n)
   {
      if (permutation(n) != n) return false;
   }
   return true;
}