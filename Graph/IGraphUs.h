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

    virtual Vertex getNumVertices() const = 0;
    virtual Edge getNumEdges() const = 0;
    virtual void setAdjacentVertices(Vertex vertex, std::vector<Vertex> &result) const = 0;
    virtual bool areAdjacent(Vertex v1, Vertex v2) const = 0;
    virtual Vertex getDegree(Vertex vertex) const = 0;
    virtual std::vector<Vertex> getConnectedComponents() const = 0;

    virtual std::string getName() const = 0;

    static auto getVertexRange(Vertex nVertices)
    {
        return stv::iota(Vertex{0}, nVertices);
    }
    auto getVertexRange() const
    {
        return getVertexRange(getNumVertices());
    }
    Vertex getNumberOfComponents() const;
    std::vector<Vertex> getDegreeSequence() const;
    std::vector<Vertex> getSortedDegreeSequence() const;
    bool isConnected() const;
    bool isComplete() const;
    std::vector<Vertex> getUniversalVertices() const;
    std::vector<std::array<Vertex, 2>> getAllSortedEdges() const;
    bool isClique(std::vector<Vertex>) const;
};
}; // namespace Graph
