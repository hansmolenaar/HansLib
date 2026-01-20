#include "GraphIsomorphismUtils.h"

#include "Defines.h"
#include "GraphIsomorphismTaggerChains.h"
#include "GraphIsomorphismTaggerColor.h"
#include "GraphIsomorphismTaggerComponents.h"
#include "GraphIsomorphismTaggerDegree.h"
#include "GraphIsomorphismTaggerDistance.h"
#include "GraphIsomorphismTaggerKnown.h"
#include "GraphIsomorphismTaggerMaxDegree.h"
#include "GraphIsomorphismTaggerNumbers.h"
#include "GraphIsomorphismTaggerTriangles.h"
#include "GraphIsomorphismTaggerTwins.h"
#include "UndirectedGraphDistance.h"

using namespace Graph;
using namespace GraphIsomorphism;

template Tag GraphIsomorphism::CondenseSizeSequence(const std::vector<int> &sizes);
template Tag GraphIsomorphism::CondenseSizeSequence(const std::vector<size_t> &sizes);

template <typename T> Tag GraphIsomorphism::CondenseSizeSequence(const std::vector<T> &sizes)
{
    std::map<T, TagEntry> count;
    for (const auto siz : sizes)
    {
        count[siz] += 1;
    }

    Tag result;
    for (const auto &itr : count)
    {
        result.push_back(itr.first);
        result.push_back(itr.second);
    }

    return result;
}

std::ostream &operator<<(std::ostream &os, const GraphIsomorphism::Tag &tag)
{
    bool first = true;
    os << "{";
    for (TagEntry entry : tag)
    {
        if (!first)
        {
            os << ",";
        }
        first = false;
        os << " " << entry;
    }
    os << " }";
    return os;
}

std::ostream &operator<<(std::ostream &os, const GraphIsomorphism::GraphTags &tags)
{
    os << "{";
    for (const auto &tag : tags)
    {
        os << " " << tag;
    }
    os << " }";
    return os;
}

std::ostream &operator<<(std::ostream &os, const std::vector<Graph::Vertex> &vertices)
{
    os << "{";
    bool first = true;
    for (auto v : vertices)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            os << ", ";
        }
        os << std::to_string(v);
    }
    os << "}";
    return os;
}

std::ostream &operator<<(std::ostream &os, const GraphIsomorphism::Grouping<Graph::Vertex> &grouping)
{
    // Print all on one line
    os << "{";
    bool first = true;
    for (auto group : grouping())
    {
        if (first)
        {
            first = false;
        }
        else
        {
            os << " ";
        }
        os << group;
    }
    os << "}\n";
    return os;
}

std::vector<std::unique_ptr<ITagger>> GraphIsomorphism::getAllTaggers(const Graph::IGraphUs &graph)
{
    std::vector<std::unique_ptr<ITagger>> result;
    auto triangles = std::make_shared<Graph::UndirectedGraphTriangles>(graph);
    auto distances = std::make_shared<Graph::UndirectedGraphDistance>(graph);

    result.emplace_back(std::make_unique<TaggerNumbers>(graph));
    result.emplace_back(std::make_unique<TaggerComponents>(graph));
    result.emplace_back(std::make_unique<TaggerDegree>(graph));
    result.emplace_back(std::make_unique<TaggerChains>(graph));
    result.emplace_back(std::make_unique<TaggerDistance>(distances, triangles));
    result.emplace_back(std::make_unique<TaggerKnown>(graph));
    result.emplace_back(std::make_unique<TaggerMaxDegree>(graph));
    result.emplace_back(std::make_unique<TaggerTriangles>(triangles));
    result.emplace_back(std::make_unique<TaggerColor>(graph));
    result.emplace_back(std::make_unique<TaggerTwins>(graph));
    return result;
}

std::vector<const IGraphTagger *> GraphIsomorphism::selectGraphTaggers(
    const std::vector<std::unique_ptr<ITagger>> &taggers)
{
    std::vector<const IGraphTagger *> result(taggers.size());
    str::transform(taggers, result.begin(), [](const auto &tagger) { return tagger->getGraphTagger(); });
    result.erase(std::remove(result.begin(), result.end(), nullptr), result.end());
    return result;
}

std::vector<const IVertexCompare *> GraphIsomorphism::selectVertexCompare(
    const std::vector<std::unique_ptr<ITagger>> &taggers)
{
    std::vector<const IVertexCompare *> result(taggers.size());
    str::transform(taggers, result.begin(), [](const auto &tagger) { return tagger->getVertexCompare(); });
    result.erase(std::remove(result.begin(), result.end(), nullptr), result.end());
    return result;
}

void GraphIsomorphism::toAdjacentyList(const Graph::IGraphUs &graph, std::ostream &os)
{
    std::vector<Vertex> ngbs;

    for (auto v : graph.getVertexRange())
    {
        graph.setAdjacentVertices(v, ngbs);
        os << v + 1 << " :";
        for (auto n : ngbs)
        {
            os << " " << n + 1;
        }
        os << "\n";
    }
}

void GraphIsomorphism::toEdgeList(const Graph::IGraphUs &graph, std::ostream &os, Graph::Vertex offset)
{
    std::vector<Vertex> ngbs;

    for (auto edge : graph.getAllSortedEdges())
    {
        os << edge[0] + offset << "-" << edge[1] + offset << "\n";
    }
}
