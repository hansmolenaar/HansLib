#pragma once

#include "IGraphIsomorphismTagger.h"
#include "IGraphUsc.h"
#include <string>
#include <vector>

namespace GraphTest
{
void CheckVertexTaggerConsistency(const Graph::IGraphUs &, GraphIsomorphism::ITaggerFactory &, int = -1, int = 5);
void CheckGraphTaggerConsistency(const Graph::IGraphUs &, GraphIsomorphism::ITaggerFactory &, int = 5);

void CheckTaggerConsistency(const Graph::IGraphUs &, GraphIsomorphism::ITaggerFactory &, int = 5, int = -1);

void CheckTagger(GraphIsomorphism::ITaggerFactory &);

void TaggerCheckListG6(const std::vector<std::string> &, GraphIsomorphism::ITaggerFactory &, int, int = 5);
} // namespace GraphTest
