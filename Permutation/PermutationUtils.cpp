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
   for (size_t n = 0; n < permutations.size(); ++n)
   {
      if (permutations[n] == identity)
      {
         return n;
      }
   }
   return {};
}

bool PermutationUtils::areUnique(std::span<const Permutation> permutations)
{
   const size_t numPermutations = permutations.size();
   for (size_t p1 = 0; p1 < numPermutations; ++p1)
   {
      for (size_t p2 = 0; p2 < numPermutations; ++p2)
      {
         if (p1 != p2)
         {
            if (permutations[p1] == permutations[p2])
            {
               return false;
            }
         }
      }
   }
   return true;
}

Permutation operator*(const Permutation& perm1, const Permutation& perm0)
{
   Utilities::MyAssert(perm1.getCardinality() == perm0.getCardinality());
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