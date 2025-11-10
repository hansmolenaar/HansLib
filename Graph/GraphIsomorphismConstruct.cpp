#include "GraphIsomorphismConstruct.h"
#include "Defines.h"
#include "GraphIsomorphismGrouper.h"
#include "GraphIsomorphismTaggerChains.h"
#include "GraphIsomorphismTaggerComponents.h"
#include "GraphIsomorphismTaggerDegree.h"
#include "GraphIsomorphismTaggerDistance.h"
#include "GraphIsomorphismTaggerKnown.h"
#include "GraphIsomorphismTaggerMaxDegree.h"
#include "GraphIsomorphismTaggerNumbers.h"
#include "MyAssert.h"

using namespace GraphIsomorphism;
using namespace Utilities;

namespace
{
TaggerNumbersFactory factoryNumbers;
TaggerComponentFactory factoryComponents;
TaggerDegreeFactory factoryDegree;
TaggerKnownFactory factoryKnown;
TaggerChainsFactory factoryChains;
TaggerMaxDegreeFactory factoryMaxDegree;
TaggerDistanceFactory factoryDistance;

const std::vector<ITaggerFactory *> factories{&factoryNumbers, &factoryComponents, &factoryDegree,  &factoryKnown,
                                              &factoryChains,  &factoryMaxDegree,  &factoryDistance};

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

std::vector<ITaggerFactory *> Construct::getTaggerFactories()
{
    return factories;
}

std::vector<std::unique_ptr<ITagger>> Construct::getAllTaggers(const Graph::IGraphUs &graph)
{
    std::vector<std::unique_ptr<ITagger>> result(factories.size());
    str::transform(factories, result.begin(), [&graph](const auto &factory) { return factory->createTagger(graph); });
    return result;
}

std::vector<const IGraphTagger *> Construct::selectGraphTaggers(const std::vector<std::unique_ptr<ITagger>> &taggers)
{
    std::vector<const IGraphTagger *> result(taggers.size());
    str::transform(taggers, result.begin(), [](const auto &tagger) { return tagger->getGraphTagger(); });
    result.erase(std::remove(result.begin(), result.end(), nullptr), result.end());
    return result;
}

std::vector<const IVertexTagger *> Construct::selectVertexTaggers(const std::vector<std::unique_ptr<ITagger>> &taggers)
{
    std::vector<const IVertexTagger *> result(taggers.size());
    str::transform(taggers, result.begin(), [](const auto &tagger) { return tagger->getVertexTagger(); });
    result.erase(std::remove(result.begin(), result.end(), nullptr), result.end());
    return result;
}

Status Construct::actionConnected(const Graph::IGraphUsc &graph0, const Graph::IGraphUsc &graph1) const
{
    const auto numVertices = graph0.getNumVertices();
    const auto factories = getTaggerFactories();

    const auto allTaggers0 = getAllTaggers(graph0);
    const auto allTaggers1 = getAllTaggers(graph1);

    Status result(numVertices);
    const auto graphTaggers0 = selectGraphTaggers(allTaggers0);
    const auto graphTaggers1 = selectGraphTaggers(allTaggers1);
    for (const auto &tagger01 : std::views::zip(graphTaggers0, graphTaggers1))
    {
        const auto &tag0 = std::get<0>(tagger01)->getGraphTag();
        const auto &tag1 = std::get<1>(tagger01)->getGraphTag();
        if (tag0 != tag1)
        {
            result.setFlag(Flag::NotIsomorphic);
            return result;
        }
    }

    std::vector<Tag> groups0(numVertices);
    std::vector<Tag> groups1(numVertices);

    const auto vertexTaggers0 = selectVertexTaggers(allTaggers0);
    const auto vertexTaggers1 = selectVertexTaggers(allTaggers1);
    for (const auto &tagger01 : std::views::zip(vertexTaggers0, vertexTaggers1))
    {

        const Grouper grouper0(*std::get<0>(tagger01));
        const Grouper grouper1(*std::get<1>(tagger01));
        if (!Grouper::areEquivalent(grouper0, grouper1))
        {
            result.setFlag(Flag::NotIsomorphic);
            return result;
        }
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
