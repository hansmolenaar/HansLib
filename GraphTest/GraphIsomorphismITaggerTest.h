#pragma once

#include "IGraphIsomorphismTagger.h"
#include "IGraphUsc.h"
#include <string>
#include <vector>

namespace GraphTest
{
void CheckTaggerConsistency(const Graph::IGraphUs &, GraphIsomorphism::ICompareFactory &, int = -1);

void CheckTagger(GraphIsomorphism::ICompareFactory &);

void CheckGraphTaggerConsistency(const Graph::IGraphUs &, GraphIsomorphism::ICompareFactory &);

} // namespace GraphTest
