#include "GraphIsomorphismComparersFactory.h"

#include "GraphIsomorphismComparers.h"
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
#include "MyAssert.h"

using namespace GraphIsomorphism;
using namespace Utilities;

namespace
{
CompareNumbersFactory compareNumbersFactory;
CompareKnownFactory compareKnownFactory;
CompareDegreeFactory compareDegreeFactory;
CompareComponentsFactory compareComponentsFactory;
CompareMaxDegreeFactory compareMaxDegreeFactory;
CompareChainsFactory compareChainsFactory;
CompareColorFactory compareColorFactory;
CompareDistanceFactory compareDistanceFactory;
CompareTrianglesFactory compareTrianglesFactory;
CompareTwinsFactory compareTwinsFactory;

std::vector<ICompareFactory *> allFactories{
    &compareNumbersFactory,   &compareKnownFactory,  &compareDegreeFactory, &compareComponentsFactory,
    &compareMaxDegreeFactory, &compareChainsFactory, &compareColorFactory,  &compareDistanceFactory,
    &compareTrianglesFactory, &compareTwinsFactory,
};

} // namespace

ComparersFactory::ComparersFactory(std::vector<ICompareFactory *> factories) : m_factories(std::move(factories))
{
    MyAssert(!m_factories.empty());
}

ComparersFactory::ComparersFactory() : ComparersFactory(getAllSimpleFactories())
{
}

std::vector<std::unique_ptr<ICompare>> ComparersFactory::getAllComparers(const Graph::IGraphUs &graph)
{
    std::vector<std::unique_ptr<ICompare>> comparers;
    for (auto *factory : m_factories)
    {
        comparers.emplace_back(factory->createCompare(graph));
    }
    return comparers;
}

std::unique_ptr<ICompare> ComparersFactory::createCompare(const Graph::IGraphUs &graph)
{
    return create(graph);
}

std::unique_ptr<Comparers> ComparersFactory::create(const Graph::IGraphUs &graph)

{
    return std::make_unique<Comparers>(getAllComparers(graph));
}

std::vector<ICompareFactory *> ComparersFactory::getAllSimpleFactories()
{
    return allFactories;
}
