#pragma once

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"

class UndirectedGraph;

namespace GraphIsomorphism
{

class Grouper
{
  public:
    GraphGroup operator()(const IGraphIsomorphismTagger &tagger);
};

} // namespace GraphIsomorphism
