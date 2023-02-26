#pragma once

#include <span>
#include "Permutation.h"

namespace PermutationUtils
{
   bool IsPermutation(std::span<const int>);
}

Permutation operator*(const Permutation&, const Permutation&);
bool operator==(const Permutation&, const Permutation&);