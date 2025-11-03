#include "GraphIsomorphismConstruct.h"
#include "Defines.h"
#include "GraphIsomorphismTaggerChains.h"
#include "GraphIsomorphismTaggerDegree.h"
#include "GraphIsomorphismTaggerDistance.h"
#include "GraphIsomorphismTaggerMaxDegree.h"
#include "GraphIsomorphismTaggerNumbers.h"
#include "MyAssert.h"

using namespace GraphIsomorphism;
using namespace Utilities;

namespace
{
TaggerNumbersFactory factoryNumbers;
TaggerDegreeFactory factoryDegree;
TaggerChainsFactory factoryChains;
TaggerMaxDegreeFactory factoryMaxDegree;
TaggerDistanceFactory factoryDistance;

const std::vector<IGraphTaggerFactory *> factoriesGraph{&factoryNumbers, &factoryDegree};
const std::vector<IVertexTaggerFactory *> factoriesVertex{&factoryChains, &factoryMaxDegree, &factoryDistance};

} // namespace

Status Construct::actionConnected(const Graph::IGraphUSC &graph0, const Graph::IGraphUSC &graph1) const
{
    Status result(graph0.getNumVertices());
    for (auto *factory : factoriesGraph)
    {
        const auto tagger0 = factory->createGraphTagger(graph0);
        const auto tagger1 = factory->createGraphTagger(graph1);
        const auto &tag0 = tagger0->getGraphTag();
        const auto &tag1 = tagger1->getGraphTag();
        if (tag0 != tag1)
        {
            result.setFlag(Flag::NotIsomorphic);
            return result;
        }
    }
    return result;
}
