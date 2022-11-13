#pragma once

#include <span>
#include "Permutaion/Permutation.h"

namespace PermutationUtils
{
   bool IsPermutation(std::span<const int>);
}

Permutation operator*(const Permutation&, const Permutation&);