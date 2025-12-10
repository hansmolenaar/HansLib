#include "IGraphIsomorphismTransform.h"

#include "GraphIsomorphismTaggedGraph.h"
#include "GraphIsomorphismTaggerKnown.h"
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

ITransform::ITransform(const TaggedGraph &tgraph) : m_taggedGraph(tgraph)
{
}

const TaggedGraph &ITransform::getTaggedGraph() const
{
    return m_taggedGraph;
}

const Graph::IGraphUs &ITransform::getGraph() const
{
    return m_taggedGraph.getGraph();
}

// !!!!!!!!!!!!! TransformKnown

TransformKnown::TransformKnown(const TaggedGraph &tgraph, const TaggerKnown &tagger)
    : ITransform(tgraph), m_tag{ITransform::Type::Known}
{
    const auto &tag = tagger.getGraphTag();
    MyAssert(tag.front() != TaggerKnown::KnownType::Unknown);
    m_tag.insert(m_tag.end(), tag.begin(), tag.end());
}

const Tag &TransformKnown::getTagOfTransform() const
{
    return m_tag;
}

std::string TransformKnown::getDescription() const
{
    return "Known graph";
}

const std::vector<const TaggedGraph *> &TransformKnown::getChildren() const
{
    return s_noChildren;
}

std::unique_ptr<ITransform> TransformKnown::tryCreate(const TaggedGraph &tgraph)
{
    const TaggerKnown taggerKnown(tgraph.getGraph());
    if (taggerKnown.getGraphTag().front() == TaggerKnown::KnownType::Unknown)
    {
        return {};
    }
    return std::unique_ptr<ITransform>(new TransformKnown(tgraph, taggerKnown));
}
