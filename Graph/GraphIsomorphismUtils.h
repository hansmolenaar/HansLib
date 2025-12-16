#pragma once

#include <vector>

#include "GraphIsomorphismDefines.h"
#include "GraphIsomorphismGrouping.h"
#include "IGraphIsomorphismTagger.h"

namespace GraphIsomorphism
{

template <typename T> GraphIsomorphism::Tag CondenseSizeSequence(const std::vector<T> &);

std::vector<std::unique_ptr<ITagger>> getAllTaggers(const Graph::IGraphUs &);
std::vector<const IGraphTagger *> selectGraphTaggers(const std::vector<std::unique_ptr<ITagger>> &);
std::vector<const IVertexCompare *> selectVertexCompare(const std::vector<std::unique_ptr<ITagger>> &);

void toAdjacentyList(const Graph::IGraphUs &, std::ostream &);
void toEdgeList(const Graph::IGraphUs &, std::ostream &, Graph::Vertex offset = 1);

} // namespace GraphIsomorphism

std::ostream &operator<<(std::ostream &, const GraphIsomorphism::Tag &);
std::ostream &operator<<(std::ostream &, const GraphIsomorphism::GraphTags &);
std::ostream &operator<<(std::ostream &, const std::vector<Graph::Vertex> &);
std::ostream &operator<<(std::ostream &, const GraphIsomorphism::Grouping<Graph::Vertex> &);
