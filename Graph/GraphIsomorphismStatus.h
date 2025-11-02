#pragma once

#include <vector>

#include "GraphIsomorphismDefines.h"

namespace GraphIsomorphism
{

class Status
{
  public:
    Status() = default;
    explicit Status(GraphVertex);
    Flag StatusFlag;
    std::vector<VertexPair> VertexPairs;

  private:
    GraphVertex m_numVertices;
};

} // namespace GraphIsomorphism
