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
    const auto numVertices = graph0.getNumVertices();
    const auto factories = getTaggerFactories();

    const auto allTaggers0 = getAllTaggers(graph0);
    const auto allTaggers1 = getAllTaggers(graph1);

    Status result(numVertices);
    const TaggedGraph tg0(graph0);
    const TaggedGraph tg1(graph1);

    if (tg0 != tg1)
    {
        result.setFlag(Flag::NotIsomorphic);
        return result;
    }

    std::vector<Tag> groups0(numVertices);
    std::vector<Tag> groups1(numVertices);

    const auto vertexTaggers0 = selectVertexTaggers(allTaggers0);
    const auto vertexTaggers1 = selectVertexTaggers(allTaggers1);
    for (const auto &tagger01 : std::views::zip(vertexTaggers0, vertexTaggers1))
    {

        const Grouper grouper0(*std::get<0>(tagger01));
        const Grouper grouper1(*std::get<1>(tagger01));

        grouper0.updateVertexGroupTags(groups0);
        grouper1.updateVertexGroupTags(groups1);

        const CombinedTagger ctagger0{groups0};
        const CombinedTagger ctagger1{groups1};
        const Grouper cgrouper0(ctagger0);
        const Grouper cgrouper1(ctagger1);

        result = Grouper::compare(cgrouper0, cgrouper1);
        if (result.getFlag() != Flag::Undecided)
        {
            return result;
        }
    }
    return result;
}
