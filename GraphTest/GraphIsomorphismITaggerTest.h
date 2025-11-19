#pragma once

#include "IGraphIsomorphismTagger.h"
#include "IGraphUsc.h"
#include <string>
#include <vector>

namespace GraphTest
{
void CheckTaggerConsistency(const Graph::IGraphUs &, GraphIsomorphism::ITaggerFactory &, int = -1);

void CheckTagger(GraphIsomorphism::ITaggerFactory &);

} // namespace GraphTest
