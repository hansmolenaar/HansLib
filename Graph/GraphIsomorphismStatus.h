#pragma once

#include <vector>

#include "GraphIsomorphismDefines.h"

namespace GraphIsomorphism
{

class Status
{
  public:
    Status() = default;
    explicit Status(Graph::GraphVertex);
    Flag getFlag() const;
    void setFlag(Flag);
    void addPair(Graph::VertexPair);
    std::vector<Graph::VertexPair> getVertexPairs() const;
    bool containsFirst(Graph::GraphVertex) const;
    bool containsSecond(Graph::GraphVertex) const;

  private:
    Graph::GraphVertex m_numVertices = 0;
    Flag m_flag = Flag::NotIsomorphic;
    std::vector<Graph::GraphVertex> m_vertexFirst;
    std::vector<Graph::GraphVertex> m_vertexSecond;
};

} // namespace GraphIsomorphism
