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
    Flag getFlag() const;
    void addPair(VertexPair);
    std::vector<VertexPair> getVertexPairs() const;

  private:
    GraphVertex m_numVertices;
    Flag m_flag;
    std::vector<GraphVertex> m_vertexFirst;
    std::vector<GraphVertex> m_vertexSecond;
};

} // namespace GraphIsomorphism
