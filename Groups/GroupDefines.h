#pragma once

#include "Permutation.h"
#include "PermutationUtils.h"

using GroupElement = Permutation::Entry;
static constexpr GroupElement GroupElementInvalid = Permutation::InvalidEntry;

template<typename T>
static GroupElement SafeCastToGroupElement(T number)
{
   return PermutationUtils::SafeCastToEntry(number);
}