#pragma once

#include "Permutation.h"
#include "MyException.h"

#include <span>
#include <optional>
#include <set>
#include <functional>
#include <numeric>
#include <string>

namespace PermutationUtils
{
   bool IsPermutation(std::span<const Permutation::Entry>);
   std::optional<Permutation::Entry> findIdentity(std::span<const Permutation> permutations);
   bool areUnique(std::span<const Permutation> permutations);
   bool isIdentity(const Permutation& permutation);
   std::set<Permutation> generateAllPowerCombinations(std::span<const Permutation> permutationsIn);

   template<typename Distance>
   static bool isIsometry(const Permutation& permutation, Distance distance)
   {
      const auto size = permutation.getCardinality();

      // Assumption: Distance is symmetric in its arguments
      for (Permutation::Entry v0 = 1; v0 < size; ++v0)
      {
         for (Permutation::Entry v1 = 0; v1 < v0; ++v1)
         {
            const auto distRef = distance(v0, v1);
            const auto distPermuted = distance(permutation(v0), permutation(v1));
            if (distRef != distPermuted) return false;
         }
      }
      return true;
   }

   template<typename T>
   Permutation::Entry SafeCastToEntry(T number)
   {
      if (number < 0) throw MyException("CastToEntry negative number " + std::to_string(number));
      if (std::cmp_greater_equal(number, std::numeric_limits<Permutation::Entry>::max())) throw MyException("CastToEntry too large number ");
      return static_cast<Permutation::Entry>(number);
   }

}

