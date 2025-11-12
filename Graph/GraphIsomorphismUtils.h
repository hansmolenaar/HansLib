#pragma once

#include <vector>

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"

namespace GraphIsomorphism
{

template <typename T> GraphIsomorphism::Tag CondenseSizeSequence(const std::vector<T> &);

    std::vector<ITaggerFactory *> getTaggerFactories();

} // namespace GraphIsomorphism

std::ostream &operator<<(std::ostream &, const GraphIsomorphism::Tag &);
std::ostream &operator<<(std::ostream &, const GraphIsomorphism::GraphTags &);
