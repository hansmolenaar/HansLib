#include "Defines.h"
#include "GroupTable.h"
#include "SymmetricGroup.h"

std::unique_ptr<IFiniteGroup> SymmetricGroup::Create(GroupElement size)
{
   std::vector<Permutation> permutations;
   std::vector<Permutation::Entry> permut(size);
   str::iota(permut, 0);
   if (size > 0)
   {
      do {
         permutations.emplace_back(Permutation::Create(permut));
      } while (str::next_permutation(permut).found);
   }

   // For permutations we don't have to check the associativity
   constexpr bool checkAssociativity = false;
   return  GroupTable::CreateUsingBinaryOperator<Permutation, decltype(Permutation::compose)>(permutations, Permutation::compose, checkAssociativity);
}