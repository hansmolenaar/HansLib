#pragma once

#include <span>
#include "Permutation.h"
#include <optional>

namespace PermutationUtils
{
   bool IsPermutation(std::span<const Permutation::Entry>);
   std::optional<Permutation::Entry> findIdentity(std::span<const Permutation> permutations);
   bool areUnique(std::span<const Permutation> permutations);
}

