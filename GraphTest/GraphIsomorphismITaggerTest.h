#pragma once

#include "IGraphIsomorphismTagger.h"
#include "IGraphUsc.h"
#include <string>
#include <vector>

namespace GraphTest
{
void CheckTaggerConsistency(const Graph::IGraphUsc &, GraphIsomorphism::IVertexTaggerFactory &, int = -1, int = 5);
void TaggerCheckListG6(const std::vector<std::string> &, GraphIsomorphism::IVertexTaggerFactory &, int, int = 5);

void CheckTaggerConsistency(const Graph::IGraphUs &, GraphIsomorphism::IGraphTaggerFactory &, int = 5);

void CheckGraphTagger(GraphIsomorphism::IGraphTaggerFactory &);
void CheckVertexTagger(GraphIsomorphism::IVertexTaggerFactory &);
} // namespace GraphTest
