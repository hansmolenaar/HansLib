#pragma once

#include <vector>

#include "GraphIsomorphismDefines.h"

namespace GraphIsomorphism
{

class Status
{
  public:
    Status() = default;
    explicit Status(Graph::Vertex);
    Flag getFlag() const;
    void setFlag(Flag);
    void addPair(Graph::VertexPair);
    std::vector<Graph::VertexPair> getVertexPairs() const;
    bool containsFirst(Graph::Vertex) const;
    bool containsSecond(Graph::Vertex) const;

  private:
    Graph::Vertex m_numVertices = 0;
    Flag m_flag = Flag::NotIsomorphic;
    std::vector<Graph::Vertex> m_vertexFirst;
    std::vector<Graph::Vertex> m_vertexSecond;
};

} // namespace GraphIsomorphism
