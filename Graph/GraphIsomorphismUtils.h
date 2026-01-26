#pragma once

#include <vector>

#include "GraphIsomorphismDefines.h"
#include "GraphIsomorphismGrouping.h"
#include "IGraphIsomorphismTagger.h"

namespace GraphIsomorphism
{

template <typename T> GraphIsomorphism::Tag CondenseSizeSequence(const std::vector<T> &);

std::vector<const IGraphTagger *> selectGraphTaggers(const std::vector<std::unique_ptr<ICompare>> &);
std::vector<const ICharacteristicsCompare *> selectCharacteristicsCompare(
    const std::vector<std::unique_ptr<ICompare>> &);
std::vector<const IVertexCompare *> selectVertexCompare(const std::vector<std::unique_ptr<ICompare>> &);

void toAdjacentyList(const Graph::IGraphUs &, std::ostream &);
void toEdgeList(const Graph::IGraphUs &, std::ostream &, Graph::Vertex offset = 1);

} // namespace GraphIsomorphism

std::ostream &operator<<(std::ostream &, const GraphIsomorphism::Tag &);
std::ostream &operator<<(std::ostream &, const std::vector<Graph::Vertex> &);
std::ostream &operator<<(std::ostream &, const GraphIsomorphism::VertexGrouping &);
