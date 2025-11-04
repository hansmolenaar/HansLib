#include "IGraphIsomorphismTagger.h"

using namespace GraphIsomorphism;

bool IGraphTaggerFactory::haveSameTags(const Graph::IGraphUs &graph0, const Graph::IGraphUs &graph1)
{
    auto tagger0 = createGraphTagger(graph0);
    auto tagger1 = createGraphTagger(graph1);
    auto tag0 = tagger0->getGraphTag();
    auto tag1 = tagger1->getGraphTag();
    return tag0 == tag1;
}
