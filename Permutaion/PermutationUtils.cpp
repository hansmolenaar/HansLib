#include "PermutationUtils.h"
#include "Utilities//Defines.h"
#include "Utilities/Assert.h"

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

Permutation operator*(const Permutation& perm1, const Permutation& perm0)
{
   Utilities::Assert(perm1.getCardinality() == perm0.getCardinality());
   const auto siz = perm0.getCardinality();
   std::vector<int> permut(siz, -1);
   for (auto n = 0; n < siz; ++n)
   {
      permut.at(n) = perm1(perm0(n));
   }
   return Permutation::Create(permut);
}


bool operator==(const Permutation& perm0, const Permutation& perm1)
{
   if (perm0.getCardinality() != perm1.getCardinality()) return false;
   const auto siz = perm0.getCardinality();
   for (auto n = 0; n < siz; ++n)
   {
      if (perm0(n) != perm1(n)) return false;
   }
   return true;
}