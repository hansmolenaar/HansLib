#include <gtest/gtest.h>

#include "GraphIsomorphismIGraphCompareTest.h"

#include "GraphIsomorphismGrouping.h"
#include "GraphIsomorphismUtils.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"
#include "UniquePointer.h"

#include <chrono>
#include <execution>

using namespace Graph;
using namespace GraphTest;
using namespace GraphIsomorphism;
using namespace Utilities;

namespace
{

std::vector<std::unique_ptr<IGraphCompare>> CreateComparers(const IGraphCompareFactory &factory,
                                                            const std::vector<std::unique_ptr<IGraphUs>> &graphs)
{
    std::vector<std::unique_ptr<IGraphCompare>> result;
    str::transform(graphs, std::back_inserter(result),
                   [&factory](const auto &upg) { return factory.createGraphCompare(*upg); });
    return result;
}

void CheckSymmetry(const IGraphCompare &compare1, const IGraphCompare &compare2)
{
    const auto cmp = compare1.compareGraph(compare2);
    if (cmp == std::weak_ordering::equivalent)
    {
        ASSERT_EQ(compare2.compareGraph(compare1), std::weak_ordering::equivalent);
    }
    else if (cmp == std::weak_ordering::less)
    {
        ASSERT_EQ(compare2.compareGraph(compare1), std::weak_ordering::greater);
    }
    else
    {
        ASSERT_EQ(compare2.compareGraph(compare1), std::weak_ordering::less);
    }
}

void CheckConsistencyList(const IGraphCompareFactory &factory, const std::vector<const IGraphCompare *> &comparers)
{
    for (const auto *cmp1 : comparers)
    {
        for (const auto *cmp2 : comparers)
        {
            CheckSymmetry(*cmp1, *cmp2);
        }
    }
}

void CheckGraphGrouping(const std::vector<const IGraphCompare *> &comparers, Tag expectMultiplicities,
                        bool printMultiples)
{
    const Grouping<const IGraphCompare *> grouping(comparers, [](const IGraphCompare *lhs, const IGraphCompare *rhs) {
        return lhs->compareGraph(*rhs) == std::weak_ordering::less;
    });
    const auto multiplicities = CondenseSizeSequence(grouping.getGroupSizes());
    ASSERT_EQ(multiplicities, expectMultiplicities);

    if (printMultiples)
    {
        for (const auto &group : grouping())
        {
            if (group.size() > 1)
            {
                std::cout << "Group of size " << group.size() << "\n";
                for (const auto &g : group)
                {
                    std::cout << g->getGraph().getName() << "\n";
                    // std::cout << g->getVertexGrouping() << "\n";
                }
                std::cout << "\n\n";
            }
        }
    }
}

} // namespace

void GraphTest::CheckList(const IGraphCompareFactory &factory,
                          const std::vector<std::unique_ptr<Graph::IGraphUs>> &graphs, Tag expectMultiplicities,
                          bool printMultiples)
{
    const auto start = std::chrono::steady_clock::now();
    std::vector<std::unique_ptr<IGraphCompare>> comparers(graphs.size());
    std::transform(std::execution::par_unseq, graphs.begin(), graphs.end(), comparers.begin(),
                   [&factory](const auto &upg) { return factory.createGraphCompare(*upg); });
    const auto finishCreation = std::chrono::steady_clock::now();
    CheckGraphGrouping(getCastPointers<const IGraphCompare>(comparers), expectMultiplicities, printMultiples);
    const auto finishGrouping = std::chrono::steady_clock::now();
    std::cout << "Creation " << std::chrono::duration_cast<std::chrono::milliseconds>(finishCreation - start)
              << " Grouping " << std::chrono::duration_cast<std::chrono::milliseconds>(finishGrouping - finishCreation)
              << '\n';
}

void GraphTest::CheckList(const IGraphCompareFactory &factory, const std::vector<std::string> &g6list,
                          Tag expectGraphTagMultiplicities, bool printMultiples)
{
    const auto graphs = UndirectedGraphFromG6::getGraphs(g6list);
    CheckList(factory, graphs, expectGraphTagMultiplicities, printMultiples);
}

void GraphTest::CheckComparerBasics(const GraphIsomorphism::IGraphCompareFactory &factory)
{
    // Test some tiny graphs
    auto graphs = UndirectedGraphFromG6::getGraphs(UndirectedGraphFromG6::getListNumVertices_3());
    graphs.emplace_back(UndirectedGraphLibrary::Get_Null());
    graphs.emplace_back(UndirectedGraphLibrary::Get_Singleton());
    graphs.emplace_back(UndirectedGraphLibrary::Get_Path(2));
    graphs.emplace_back(UndirectedGraphLibrary::Get_DisconnectedGraph(2));

    const auto comparers = CreateComparers(factory, graphs);

    CheckConsistencyList(factory, getCastPointers<const IGraphCompare>(comparers));
}
