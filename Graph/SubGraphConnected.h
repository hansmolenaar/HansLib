#pragma once

#include "IGraphUsc.h"
#include "SubGraph.h"

namespace Graph
{

class SubGraphConnected : public SubGraph
{
  public:
    SubGraphConnected(const IGraphUs &, const std::set<GraphVertex> &);

};
} // namespace Graph
