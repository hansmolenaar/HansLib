#include "IGraphIsomorphismTransform.h"

#include "GraphIsomorphismXGraph.h"
#include "IGraphUs.h"
#include "MyAssert.h"
#include "UndirectedGraph.h"

using namespace GraphIsomorphism;
using namespace Graph;
using namespace Utilities;

namespace
{
const std::vector<std::shared_ptr<XGraph>> s_noChildren;
}

// !!!!!!!!!!!!! ITransform

ITransform::ITransform(std::shared_ptr<XGraph> xgraph) : m_xgraph(std::move(xgraph))
{
}

std::unique_ptr<ITransform> ITransform::Create(const std::shared_ptr<XGraph> &xgraph)
{
    auto transformKnown = TransformKnown::tryCreate(xgraph);
    if (transformKnown)
    {
        return transformKnown;
    }

    auto transformDisconnected = TransformDisconnected::tryCreate(xgraph);
    if (transformDisconnected)
    {
        return transformDisconnected;
    }

    auto transformComplementDisconnected = TransformComplementDisconnected::tryCreate(xgraph);
    if (transformComplementDisconnected)
    {
        return transformComplementDisconnected;
    }

    auto transformComplementKnown = TransformComplementKnown::tryCreate(xgraph);
    if (transformComplementKnown)
    {
        return transformComplementKnown;
    }

    auto transformOmitEdges = TransformOmitEdges::tryCreate(xgraph);
    if (transformOmitEdges)
    {
        return transformOmitEdges;
    }

    // Return failure
    return std::make_unique<TransformFailure>(xgraph);
}

const XGraph &ITransform::getXGraph() const
{
    return *m_xgraph;
}

const Graph::IGraphUs &ITransform::getGraph() const
{
    return m_xgraph->getGraph();
}

// !!!!!!!!!!!!! TransformKnown

TransformKnown::TransformKnown(const std::shared_ptr<XGraph> &xgraph, TaggerKnown tagger)
    : ITransform(xgraph), m_taggerKnown(std::move(tagger)), m_tag{ITransform::Type::Known}
{
    const auto &tag = m_taggerKnown.getGraphTag();
    MyAssert(tag.front() != TaggerKnown::KnownType::Unknown);
    m_tag.insert(m_tag.end(), tag.begin(), tag.end());
}

const Tag &TransformKnown::getTagOfTransform() const
{
    return m_tag;
}

std::string TransformKnown::getDescription() const
{
    return "Known graph: " + m_taggerKnown.getDescription();
}

const std::vector<std::shared_ptr<XGraph>> &TransformKnown::getChildren() const
{
    return s_noChildren;
}

std::unique_ptr<TransformKnown> TransformKnown::tryCreate(const std::shared_ptr<XGraph> &xgraph)
{
    const TaggerKnown taggerKnown(xgraph->getGraph());
    if (taggerKnown.getGraphTag().front() == TaggerKnown::KnownType::Unknown)
    {
        return {};
    }
    return std::unique_ptr<TransformKnown>(new TransformKnown(xgraph, taggerKnown));
}

// !!!!!!!!!!!!! TransformFailure

TransformFailure::TransformFailure(const std::shared_ptr<XGraph> &xgraph)
    : ITransform(xgraph), m_tag{ITransform::Type::Failure}
{
}

const Tag &TransformFailure::getTagOfTransform() const
{
    return m_tag;
}

std::string TransformFailure::getDescription() const
{
    return "Graph of order " + std::to_string(getGraph().getNumVertices()) + " cannot be transformed";
}

const std::vector<std::shared_ptr<XGraph>> &TransformFailure::getChildren() const
{
    return s_noChildren;
}

// !!!!!!!!!!!!! TransformDisconnected
std::unique_ptr<TransformDisconnected> TransformDisconnected::tryCreate(const std::shared_ptr<XGraph> &xgraph)
{
    const auto components = getComponentsJoinSingletons(xgraph->getGraph());
    if (components.size() < 2)
    {
        return {};
    }

    return std::unique_ptr<TransformDisconnected>(new TransformDisconnected(xgraph, components));
}

TransformDisconnected::TransformDisconnected(const std::shared_ptr<XGraph> &xgraph,
                                             const std::vector<std::vector<Graph::Vertex>> &components)
    : ITransform(xgraph), m_tag{ITransform::Type::Disconnected}
{
    const auto &graph = xgraph->getGraph();
    MyAssert(components.size() > 1);

    for (const auto &cmp : components)
    {
        m_components.emplace_back(std::make_unique<SubGraph>(graph, std::set<Vertex>(cmp.begin(), cmp.end())));
        m_xgraph.emplace_back(std::make_shared<XGraph>(*m_components.back()));
        m_tag.push_back(cmp.size());
    }
    std::sort(m_tag.begin() + 1, m_tag.end());
}

const Tag &TransformDisconnected::getTagOfTransform() const
{
    return m_tag;
}

const std::vector<std::shared_ptr<XGraph>> &TransformDisconnected::getChildren() const
{
    return m_xgraph;
}

std::string TransformDisconnected::getDescription() const
{
    std::string result{"Disconnected graph with components of order:"};
    for (auto itr = m_tag.begin() + 1; itr != m_tag.end(); ++itr)
    {
        result += " " + std::to_string(*itr);
    }
    return result;
}

std::vector<std::vector<Graph::Vertex>> TransformDisconnected::getComponentsJoinSingletons(const Graph::IGraphUs &graph)
{
    const auto numVertices = graph.getNumVertices();
    if (numVertices == 0)
    {
        return {};
    }

    const auto components = graph.getConnectedComponents();
    const auto numComponents = *str::max_element(components) + 1;
    std::vector<std::vector<Vertex>> result(numComponents);

    for (Vertex v = 0; v < numVertices; ++v)
    {
        result[components.at(v)].push_back(v);
    }

    // Collect singletons
    std::vector<Vertex> singletons;
    for (const auto &comp : result)
    {
        if (comp.size() == 1)
        {
            singletons.push_back(comp.front());
        }
    }

    // Join singletons
    if (singletons.size() > 1)
    {
        std::erase_if(result, [](const std::vector<Vertex> &comp) { return comp.size() == 1; });
        result.push_back(singletons);
    }

    return result;
}

// !!!!!!!!!!!!! TransformComplemntDisconnected

std::unique_ptr<TransformComplementDisconnected> TransformComplementDisconnected::tryCreate(
    const std::shared_ptr<XGraph> &xgraph)
{
    const auto &complement = xgraph->getGraphComplement();
    const auto components = TransformDisconnected::getComponentsJoinSingletons(complement);
    if (components.size() < 2)
    {
        return {};
    }

    return std::unique_ptr<TransformComplementDisconnected>(new TransformComplementDisconnected(xgraph, components));
}

TransformComplementDisconnected::TransformComplementDisconnected(
    const std::shared_ptr<XGraph> &xgraph, const std::vector<std::vector<Graph::Vertex>> &components)
    : ITransform(xgraph), m_tag{ITransform::Type::ComplementDisconnected}
{
    const auto &graph = xgraph->getGraph();
    MyAssert(components.size() > 1);

    for (const auto &cmp : components)
    {
        m_components.emplace_back(std::make_unique<SubGraph>(graph, std::set<Vertex>(cmp.begin(), cmp.end())));
        m_xgraph.emplace_back(std::make_shared<XGraph>(*m_components.back()));
        m_tag.push_back(cmp.size());
    }
    std::sort(m_tag.begin() + 1, m_tag.end());
}

const Tag &TransformComplementDisconnected::getTagOfTransform() const
{
    return m_tag;
}

const std::vector<std::shared_ptr<XGraph>> &TransformComplementDisconnected::getChildren() const
{
    return m_xgraph;
}

std::string TransformComplementDisconnected::getDescription() const
{
    std::string result{"Complement is disconnected graph with components of order:"};
    for (auto itr = m_tag.begin() + 1; itr != m_tag.end(); ++itr)
    {
        result += " " + std::to_string(*itr);
    }
    return result;
}

// !!!!!!!!!!!!! TransformComplemntKnown

std::unique_ptr<TransformComplementKnown> TransformComplementKnown::tryCreate(const std::shared_ptr<XGraph> &xgraph)
{
    const auto &complement = xgraph->getGraphComplement();
    const TaggerKnown taggerKnown(complement);
    if (taggerKnown.getGraphTag().front() == TaggerKnown::KnownType::Unknown)
    {
        return {};
    }
    return std::unique_ptr<TransformComplementKnown>(new TransformComplementKnown(xgraph, taggerKnown));
}

TransformComplementKnown::TransformComplementKnown(const std::shared_ptr<XGraph> &xgraph, const TaggerKnown &tagger)
    : ITransform(xgraph), m_taggerKnown(std::move(tagger)), m_tag{ITransform::Type::ComplementKnown}
{
    const auto &tag = m_taggerKnown.getGraphTag();
    MyAssert(tag.front() != TaggerKnown::KnownType::Unknown);
    m_tag.insert(m_tag.end(), tag.begin(), tag.end());
}

const Tag &TransformComplementKnown::getTagOfTransform() const
{
    return m_tag;
}

const std::vector<std::shared_ptr<XGraph>> &TransformComplementKnown::getChildren() const
{
    return s_noChildren;
}

std::string TransformComplementKnown::getDescription() const
{
    return "Complement is known graph: " + m_taggerKnown.getDescription();
}

// !!!!!!!!!!!!! TransformOmitEdges
std::unique_ptr<TransformOmitEdges> TransformOmitEdges::tryCreate(const std::shared_ptr<XGraph> &xgraph)
{
    const Grouping<Graph::Vertex> &vertexGrouping = xgraph->getVertexGrouping();
    std::vector<std::vector<Vertex>> cliques;

    for (const auto &group : vertexGrouping())
    {
        if (xgraph->getGraph().isClique(group))
        {
            cliques.emplace_back(group);
        }
    }

    if (cliques.empty())
    {
        return {};
    }

    auto retval = std::unique_ptr<TransformOmitEdges>(new TransformOmitEdges(xgraph, cliques));
    if (retval->m_numOmittedEdges == 0)
    {
        return {};
    }
    return retval;
}

TransformOmitEdges::TransformOmitEdges(const std::shared_ptr<XGraph> &xgraph,
                                       const std::vector<std::vector<Graph::Vertex>> &cliques)
    : ITransform(xgraph), m_tag{OmitEdges}
{
    const auto &graph = xgraph->getGraph();
    m_child = std::make_unique<UndirectedGraph>(UndirectedGraph::CreateEdgesOmitted(graph, cliques));
    m_numOmittedEdges = graph.getNumEdges() - m_child->getNumEdges();
    if (m_numOmittedEdges == 0)
    {
        return;
    }

    m_tag.push_back(m_numOmittedEdges);
    m_xgraph.emplace_back(std::make_shared<XGraph>(*m_child));
}
const Tag &TransformOmitEdges::getTagOfTransform() const
{
    return m_tag;
}

std::string TransformOmitEdges::getDescription() const
{
    return "Omit " + std::to_string(m_numOmittedEdges) + " edges";
}

const std::vector<std::shared_ptr<XGraph>> &TransformOmitEdges::getChildren() const
{
    return m_xgraph;
}
