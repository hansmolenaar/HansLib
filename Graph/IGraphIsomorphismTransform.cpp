#include "IGraphIsomorphismTransform.h"

#include "GraphIsomorphismTaggedGraph.h"
#include "IGraphUs.h"
#include "MyAssert.h"

using namespace GraphIsomorphism;
using namespace Graph;
using namespace Utilities;

namespace
{
const std::vector<std::shared_ptr<TaggedGraph>> s_noChildren;
}

// !!!!!!!!!!!!! ITransform

ITransform::ITransform(std::shared_ptr<TaggedGraph> tgraph) : m_taggedGraph(std::move(tgraph))
{
}

std::unique_ptr<ITransform> ITransform::Create(const std::shared_ptr<TaggedGraph> &tgraph)
{
    auto transformKnown = TransformKnown::tryCreate(tgraph);
    if (transformKnown)
    {
        return transformKnown;
    }

    auto transformDisconnected = TransformDisconnected::tryCreate(tgraph);
    if (transformDisconnected)
    {
        return transformDisconnected;
    }

    // Return failure
    return std::make_unique<TransformFailure>(tgraph);
}

const TaggedGraph &ITransform::getTaggedGraph() const
{
    return *m_taggedGraph;
}

const Graph::IGraphUs &ITransform::getGraph() const
{
    return m_taggedGraph->getGraph();
}

const std::shared_ptr<TaggedGraph> ITransform::getTaggedGraphPtr() const
{
    return m_taggedGraph;
}
// !!!!!!!!!!!!! TransformKnown

TransformKnown::TransformKnown(const std::shared_ptr<TaggedGraph> &tgraph, TaggerKnown tagger)
    : ITransform(tgraph), m_taggerKnown(std::move(tagger)), m_tag{ITransform::Type::Known}
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

const std::vector<std::shared_ptr<TaggedGraph>> &TransformKnown::getChildren() const
{
    return s_noChildren;
}

std::unique_ptr<TransformKnown> TransformKnown::tryCreate(const std::shared_ptr<TaggedGraph> &tgraph)
{
    const TaggerKnown taggerKnown(tgraph->getGraph());
    if (taggerKnown.getGraphTag().front() == TaggerKnown::KnownType::Unknown)
    {
        return {};
    }
    return std::unique_ptr<TransformKnown>(new TransformKnown(tgraph, taggerKnown));
}

// !!!!!!!!!!!!! TransformFailure

TransformFailure::TransformFailure(const std::shared_ptr<TaggedGraph> &tgraph)
    : ITransform(tgraph), m_tag{ITransform::Type::Failure}
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

const std::vector<std::shared_ptr<TaggedGraph>> &TransformFailure::getChildren() const
{
    return s_noChildren;
}

// !!!!!!!!!!!!! TransformDisconnected
std::unique_ptr<TransformDisconnected> TransformDisconnected::tryCreate(const std::shared_ptr<TaggedGraph> &tgraph)
{
    if (tgraph->getGraph().isConnected())
    {
        return {};
    }

    return std::unique_ptr<TransformDisconnected>(new TransformDisconnected(tgraph));
}

TransformDisconnected::TransformDisconnected(const std::shared_ptr<TaggedGraph> &tgraph)
    : ITransform(tgraph), m_tag{ITransform::Type::Disconnected}
{
    const auto &graph = tgraph->getGraph();
    std::map<Vertex, std::set<Vertex>> components;
    Vertex vertex = 0;
    for (auto c : graph.getConnectedComponents())
    {
        components[c].insert(vertex);
        ++vertex;
    }
    MyAssert(components.size() > 1);

    for (const auto &cmp : components)
    {
        m_components.emplace_back(std::make_unique<SubGraph>(graph, cmp.second));
        m_taggedGraphs.emplace_back(std::make_shared<TaggedGraph>(*m_components.back()));
        m_tag.push_back(cmp.second.size());
    }
    std::sort(m_tag.begin() + 1, m_tag.end());
}

const Tag &TransformDisconnected::getTagOfTransform() const
{
    return m_tag;
}

const std::vector<std::shared_ptr<TaggedGraph>> &TransformDisconnected::getChildren() const
{
    return m_taggedGraphs;
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
