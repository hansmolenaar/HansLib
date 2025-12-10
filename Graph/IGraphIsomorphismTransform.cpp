#include "IGraphIsomorphismTransform.h"

#include "GraphIsomorphismTaggedGraph.h"
#include "IGraphUs.h"
#include "MyAssert.h"

using namespace GraphIsomorphism;
using namespace Graph;
using namespace Utilities;

namespace
{
const std::vector<const TaggedGraph *> s_noChildren;
}

// !!!!!!!!!!!!! ITransform

ITransform::ITransform(std::shared_ptr<TaggedGraph> tgraph) : m_taggedGraph(std::move(tgraph))
{
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

const std::vector<const TaggedGraph *> &TransformKnown::getChildren() const
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
