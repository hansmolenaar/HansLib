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
#include "GraphIsomorphismTaggerShortestCycle.h"
#include "GraphIsomorphismTaggerSpectrum.h"
#include "GraphIsomorphismTaggerTriangles.h"
#include "GraphIsomorphismTaggerTwins.h"
#include "MyAssert.h"

using namespace GraphIsomorphism;
using namespace Utilities;

namespace
{
const CompareNumbersFactory compareNumbersFactory;
const CompareKnownFactory compareKnownFactory;
const CompareDegreeFactory compareDegreeFactory;
const CompareComponentsFactory compareComponentsFactory;
const CompareMaxDegreeFactory compareMaxDegreeFactory;
const CompareChainsFactory compareChainsFactory;
const CompareColorFactory compareColorFactory;
const CompareDistanceFactory compareDistanceFactory;
const CompareTrianglesFactory compareTrianglesFactory;
const CompareTwinsFactory compareTwinsFactory;
const CompareShortestCycleFactory compareShortestCycleFactory;
const CompareSpectrumFactory compareSpectrumFactory;

std::vector<const ICompareFactory *> allFactories{
    &compareNumbersFactory,   &compareKnownFactory,  &compareDegreeFactory,        &compareComponentsFactory,
    &compareMaxDegreeFactory, &compareChainsFactory, &compareColorFactory,         &compareDistanceFactory,
    &compareTrianglesFactory, &compareTwinsFactory,  &compareShortestCycleFactory, // &compareSpectrumFactory,
};

} // namespace

ComparersFactory::ComparersFactory(std::vector<const ICompareFactory *> factories) : m_factories(std::move(factories))
{
    MyAssert(!m_factories.empty());
}

ComparersFactory::ComparersFactory() : ComparersFactory(getAllSimpleFactories())
{
}

std::vector<std::unique_ptr<ICompare>> ComparersFactory::getAllComparers(const Graph::IGraphUs &graph) const
{
    std::vector<std::unique_ptr<ICompare>> comparers;
    for (auto *factory : m_factories)
    {
        comparers.emplace_back(factory->createCompare(graph));
    }
    return comparers;
}

std::unique_ptr<ICompare> ComparersFactory::createCompare(const Graph::IGraphUs &graph) const
{
    return create(graph);
}

std::unique_ptr<Comparers> ComparersFactory::create(const Graph::IGraphUs &graph) const

{
    return std::make_unique<Comparers>(getAllComparers(graph));
}

std::vector<const ICompareFactory *> ComparersFactory::getAllSimpleFactories()
{
    return allFactories;
}
