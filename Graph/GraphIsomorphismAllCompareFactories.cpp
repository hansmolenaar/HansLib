#include "GraphIsomorphismAllCompareFactories.h"

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
#include "IGraphIsomorphismTagger.h"
#include "MyAssert.h"

using namespace GraphIsomorphism;

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

std::vector<std::unique_ptr<ICompare>> AllCompareFactories::getAllComparers(const Graph::IGraphUs &graph) const
{
    constexpr size_t expectedNumFactories = 10;
    Utilities::MyAssert(allFactories.size() == expectedNumFactories);

    std::vector<std::unique_ptr<ICompare>> result(expectedNumFactories);
    str::transform(allFactories, result.begin(), [&graph](const auto &f) { return f->createCompare(graph); });
    return result;
};
