#pragma once

#include <span>
#include "Permutation.h"
#include <optional>

namespace PermutationUtils
{
   bool IsPermutation(std::span<const int>);
   std::optional<size_t> findIdentity(std::span<const Permutation> permutations);
   bool areUnique(std::span<const Permutation> permutations);
}

Permutation operator*(const Permutation&, const Permutation&);
bool operator==(const Permutation&, const Permutation&);