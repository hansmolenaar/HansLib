#pragma once

#include "IGraphIsomorphismTagger.h"
#include "IGraphUsc.h"
#include <string>
#include <vector>

namespace GraphTest
{
void CheckTaggerConsistency(const Graph::IGraphUs &, GraphIsomorphism::ITaggerFactory &, int = -1);

void CheckTagger(GraphIsomorphism::ITaggerFactory &);

void TaggerCheckListG6(const std::vector<std::string> &, GraphIsomorphism::ITaggerFactory &, int);
} // namespace GraphTest
