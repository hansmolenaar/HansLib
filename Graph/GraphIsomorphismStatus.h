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
    void setFlag(Flag);
    void addPair(Graph::VertexPair);
    std::vector<Graph::VertexPair> getVertexPairs() const;
    bool containsFirst(GraphVertex) const;
    bool containsSecond(GraphVertex) const;

  private:
    GraphVertex m_numVertices = 0;
    Flag m_flag = Flag::NotIsomorphic;
    std::vector<GraphVertex> m_vertexFirst;
    std::vector<GraphVertex> m_vertexSecond;
};

} // namespace GraphIsomorphism
