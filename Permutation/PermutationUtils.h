#pragma once

#include "Permutation.h"

#include <span>
#include <optional>
#include <set>

namespace PermutationUtils
{
   bool IsPermutation(std::span<const Permutation::Entry>);
   std::optional<Permutation::Entry> findIdentity(std::span<const Permutation> permutations);
   bool areUnique(std::span<const Permutation> permutations);
   bool isIdentity(const Permutation& permutation);
   std::set<Permutation> generateAllPowerCombinations(std::span<const Permutation> permutationsIn);
}

