#pragma once

#include "IGraphIsomorphismTagger.h"
#include "IGraphUsc.h"
#include <string>
#include <vector>

namespace GraphTest
{

void CheckList(GraphIsomorphism::ICompareFactory &, const std::vector<std::string> &, GraphIsomorphism::Tag,
               bool printMultiples = false);
void CheckList(GraphIsomorphism::ICompareFactory &, const std::vector<std::unique_ptr<Graph::IGraphUs>> &,
               GraphIsomorphism::Tag, bool printMultiples = false);

void CheckVertexCompareConsistency(const Graph::IGraphUs &, GraphIsomorphism::ICompareFactory &, int = -1);

void CheckComparerBasics(GraphIsomorphism::ICompareFactory &, GraphIsomorphism::Tag);

} // namespace GraphTest
