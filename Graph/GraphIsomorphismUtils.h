#pragma once

#include <vector>

#include "GraphIsomorphismDefines.h"

namespace GraphIsomorphism
{

template <typename T> GraphIsomorphism::Tag CondenseSizeSequence(const std::vector<T> &);

} // namespace GraphIsomorphism

std::ostream &operator<<(std::ostream &, const GraphIsomorphism::Tag &);
std::ostream &operator<<(std::ostream &, const GraphIsomorphism::GraphTags &);
