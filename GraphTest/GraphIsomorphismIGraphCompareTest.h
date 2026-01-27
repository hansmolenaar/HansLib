#pragma once

#include "IGraphIsomorphismTagger.h"
#include "IGraphUsc.h"
#include <string>
#include <vector>

namespace GraphTest
{

#if false
void CheckList(GraphIsomorphism::ICompareFactory &, const std::vector<std::string> &, GraphIsomorphism::Tag);
void CheckList(GraphIsomorphism::ICompareFactory &, const std::vector<std::unique_ptr<Graph::IGraphUs>> &,
               GraphIsomorphism::Tag);

void CheckVertexCompareConsistency(const Graph::IGraphUs &, GraphIsomorphism::ICompareFactory &, int = -1);
void CheckGraphTaggerConsistency(const Graph::IGraphUs &, GraphIsomorphism::ICompareFactory &);

void CheckComparerBasics(GraphIsomorphism::ICompareFactory &, GraphIsomorphism::Tag);
#endif

} // namespace GraphTest
