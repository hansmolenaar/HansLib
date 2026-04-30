#include <gtest/gtest.h>

#include "Defines.h"
#include "Functors.h"
#include "GraphIsomorphismICompareTest.h"
#include "GraphIsomorphismTaggerSpectrum.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"
#include "UndirectedGraphSpectrum.h"

#include <iostream>

using namespace GraphIsomorphism;
using namespace Graph;

TEST(GraphIsomorphismTaggerSpectrumTest, CheckTagger)
{
    CompareSpectrumFactory factory;
    GraphTest::CheckComparerBasics(factory, Tag{1, 6, 3, 1});
}

TEST(GraphIsomorphismTaggerSpectrumTest, CheckList)
{
    CompareSpectrumFactory factory;
    GraphTest::CheckList(factory, UndirectedGraphFromG6::getListNumVertices_5(), Tag{1, 4, 2, 4, 4, 1, 5, 1, 13, 1});
}

TEST(GraphIsomorphismTaggerSpectrumTest, CheckList4)
{
    CompareSpectrumFactory factory;
    std::vector<std::unique_ptr<IGraphUs>> list;
    list.emplace_back(UndirectedGraphLibrary::Get_Diamond());
    list.emplace_back(UndirectedGraphLibrary::Get_CompleteGraph(4));
    list.emplace_back(UndirectedGraphLibrary::Get_Claw());
    list.emplace_back(UndirectedGraphFromG6::Create(UndirectedGraphFromG6::pan3));
    list.emplace_back(UndirectedGraphLibrary::Get_Cycle(4));
    std::vector<std::unique_ptr<TaggerSpectrum>> taggers(list.size());
    str::transform(list, taggers.begin(), [](const auto &gp) { return std::make_unique<TaggerSpectrum>(*gp); });
    for (const auto &spectrum : taggers)
    {
        std::cout << *spectrum->getEigenSolution() << "\n";
    }
    GraphTest::CheckList(factory, list, Tag{5, 1});
}

TEST(GraphIsomorphismTaggerSpectrumTest, IsoSpectral)
{
    Functors::AreClose areClose{.RelTolerance = 1.0e-2, .AbsTolerance = 1.0e-5};
    const std::array<double, 6> expect{0, 3 - std::sqrt(5.0), 2, 3, 3, 3 + std::sqrt(5.0)};
    auto g1 = UndirectedGraph::CreateDisjointedUnion(*UndirectedGraphLibrary::Get_Cycle(4),
                                                     *UndirectedGraphLibrary::Get_Path(2));
    g1.addEdge(0, 4);
    g1.addEdge(0, 5);

    auto g2 = UndirectedGraph::CreateDisjointedUnion(*UndirectedGraphLibrary::Get_Cycle(4),
                                                     *UndirectedGraphLibrary::Get_Path(2));
    g2.addEdge(0, 4);
    g2.addEdge(2, 4);
    const TaggerSpectrum tagger1(g1);
    const TaggerSpectrum tagger2(g2);

    const auto spectrum1 = tagger1.getEigenSolution();
    const auto spectrum2 = tagger2.getEigenSolution();
    const auto eigenValues1 = spectrum1->getAvailableEigenValues();
    const auto eigenValues2 = spectrum2->getAvailableEigenValues();

    ASSERT_TRUE(str::equal(eigenValues1, expect, areClose));
    ASSERT_TRUE(str::equal(eigenValues2, expect, areClose));

    ASSERT_EQ(tagger1.compareGraph(tagger2), std::weak_ordering::equivalent);
}
