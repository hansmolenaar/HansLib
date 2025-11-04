#pragma once

#include "IGraphIsomorphismTagger.h"
#include "IGraphUSC.h"
#include <string>
#include <vector>

namespace GraphTest
{
void CheckTaggerConsistency(const Graph::IGraphUSC &, GraphIsomorphism::IVertexTaggerFactory &, int = -1, int = 5);
void TaggerCheckListG6(const std::vector<std::string> &, GraphIsomorphism::IVertexTaggerFactory &, int, int = 5);

void CheckTaggerConsistency(const Graph::IGraphUS &, GraphIsomorphism::IGraphTaggerFactory &, int = 5);
void CheckGraphTagger(GraphIsomorphism::IGraphTaggerFactory &);
} // namespace GraphTest
