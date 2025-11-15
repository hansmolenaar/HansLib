#include "GraphIsomorphismConstruct.h"

#include "Defines.h"
#include "GraphIsomorphismGrouper.h"
#include "GraphIsomorphismTaggedGraph.h"
#include "GraphIsomorphismUtils.h"
#include "MyAssert.h"

using namespace GraphIsomorphism;
using namespace Utilities;

namespace
{

class CombinedTagger : public IVertexTagger
{
  public:
    explicit CombinedTagger(const std::vector<Tag> &tags) : m_tags(tags)
    {
    }
    const Tag &getVertexTag(GraphVertex v) const override
    {
        return m_tags.at(v);
    };
    GraphVertex getNumVertices() const override
    {
        return m_tags.size();
    };

  private:
    const std::vector<Tag> &m_tags;
};

} // namespace

Status Construct::actionConnected(const Graph::IGraphUsc &graph0, const Graph::IGraphUsc &graph1) const
{
    const TaggedGraph tg0(graph0);
    const TaggedGraph tg1(graph1);
    return TaggedGraph::tryConnect(tg0, tg1);
}
