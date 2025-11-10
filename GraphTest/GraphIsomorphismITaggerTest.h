#pragma once

#include "IGraphIsomorphismTagger.h"
#include "IGraphUsc.h"
#include <string>
#include <vector>

namespace GraphTest
{
void CheckVertexTaggerConsistency(const Graph::IGraphUs &, GraphIsomorphism::ITaggerFactory &, int = -1, int = 5);
void TaggerCheckListG6(const std::vector<std::string> &, GraphIsomorphism::ITaggerFactory &, int, int = 5);

void CheckGraphTaggerConsistency(const Graph::IGraphUs &, GraphIsomorphism::ITaggerFactory &, int = 5);

void CheckGraphTagger(GraphIsomorphism::ITaggerFactory &);
void CheckVertexTagger(GraphIsomorphism::ITaggerFactory &);
} // namespace GraphTest
