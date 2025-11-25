#pragma once

#include "Defines.h"
#include "GraphDefines.h"

#include <string>

namespace Graph
{

// Undirected, Simple Graph
// Simple -> no self connections, no 'double' connections
class IGraphUs
{
  public:
    virtual ~IGraphUs() = default;

    virtual GraphVertex getNumVertices() const = 0;
    virtual GraphEdge getNumEdges() const = 0;
    virtual void setAdjacentVertices(GraphVertex vertex, std::vector<GraphVertex> &result) const = 0;
    virtual bool areAdjacent(GraphVertex v1, GraphVertex v2) const = 0;
    virtual GraphVertex getDegree(GraphVertex vertex) const = 0;
    virtual std::vector<GraphVertex> getConnectedComponents() const = 0;

    virtual std::string getName() const = 0;

    static auto getVertexRange(GraphVertex nVertices)
    {
        return stv::iota(GraphVertex{0}, nVertices);
    }
    auto getVertexRange() const
    {
        return getVertexRange(getNumVertices());
    }
    GraphVertex getNumberOfComponents() const;
    std::vector<GraphVertex> getDegreeSequence() const;
    std::vector<GraphVertex> getSortedDegreeSequence() const;
    bool isConnected() const;
    bool isComplete() const;
    std::vector<GraphVertex> getFullyConnectedVertices() const;
    std::vector<std::array<GraphVertex, 2>> getAllSortedEdges() const;
    bool isClique(std::vector<GraphVertex>) const;
};
}; // namespace Graph
