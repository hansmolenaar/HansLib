#pragma once

#include "IGraphIsomorphismTagger.h"

namespace GraphTest
{
void CheckTaggerConsistency(const UndirectedGraph &, GraphIsomorphism::ITaggerFactory &, int = -1, int = 10);
}
