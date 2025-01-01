#include "AlternatingGroup.h"
#include "GroupTable.h"


std::unique_ptr<IFiniteGroup> AlternatingGroup::Create(GroupElement size)
{
   std::vector<Permutation> permutations;
   std::vector<Permutation::Entry> permut(size);
   str::iota(permut, 0);

   if (size > 0)
   {
      do {
         auto p = Permutation::Create(permut);
         if (p.getParity() == Permutation::Parity::EVEN)
         {
            permutations.emplace_back(std::move(p));
         }
      } while (str::next_permutation(permut).found);
   }

   // For permutations we don't have to check the associativity
   constexpr bool checkAssociativity = false;
   return  GroupTable::CreateUsingBinaryOperator<Permutation, decltype(Permutation::compose)>(permutations, Permutation::compose, checkAssociativity);
}