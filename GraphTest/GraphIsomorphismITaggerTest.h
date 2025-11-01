#pragma once

#include "IGraphIsomorphismTagger.h"
#include <string>
#include <vector>

namespace GraphTest
{
void CheckTaggerConsistency(const UndirectedGraph &, GraphIsomorphism::ITaggerFactory &, int = -1, int = 5);
void TaggerCheckListG6(const std::vector<std::string> &, GraphIsomorphism::ITaggerFactory &, int, int = 5);

void CheckTaggerConsistency(const Graph::IGraphUS &, GraphIsomorphism::IGraphTaggerFactory &, int = -1, int = 5);
} // namespace GraphTest
