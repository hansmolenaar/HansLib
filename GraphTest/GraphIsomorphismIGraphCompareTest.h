#pragma once

#include "IGraphIsomorphismTagger.h"
#include "IGraphUsc.h"
#include <string>
#include <vector>

namespace GraphTest
{

void CheckComparerBasics(const GraphIsomorphism::IGraphCompareFactory &);
void CheckList(const GraphIsomorphism::IGraphCompareFactory &, const std::vector<std::string> &, GraphIsomorphism::Tag,
               bool printMultiples = false);
void CheckList(const GraphIsomorphism::IGraphCompareFactory &, const std::vector<std::unique_ptr<Graph::IGraphUs>> &,
               GraphIsomorphism::Tag, bool printMultiples = false);

} // namespace GraphTest
