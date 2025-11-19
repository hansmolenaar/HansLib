#pragma once

#include <vector>

#include "GraphIsomorphismDefines.h"
#include "IGraphIsomorphismTagger.h"
#include "IGraphIsomorphismVertexCompare.h"

namespace GraphIsomorphism
{

template <typename T> GraphIsomorphism::Tag CondenseSizeSequence(const std::vector<T> &);

std::vector<ITaggerFactory *> getTaggerFactories();
std::vector<std::unique_ptr<ITagger>> getAllTaggers(const Graph::IGraphUs &);
std::vector<const IGraphTagger *> selectGraphTaggers(const std::vector<std::unique_ptr<ITagger>> &);
std::vector<const IVertexTagger *> selectVertexTaggers(const std::vector<std::unique_ptr<ITagger>> &);
std::vector<const IVertexCompare *> selectVertexCompare(const std::vector<std::unique_ptr<ITagger>> &);

void toAdjacentyList(const Graph::IGraphUs &, std::ostream &);
void toEdgeList(const Graph::IGraphUs &, std::ostream &);

} // namespace GraphIsomorphism

std::ostream &operator<<(std::ostream &, const GraphIsomorphism::Tag &);
std::ostream &operator<<(std::ostream &, const GraphIsomorphism::GraphTags &);
