#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismIGraphCompareTest.h"
#include "GraphIsomorphismTaggerSpectrum.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

#include <iostream>

using namespace GraphIsomorphism;
using namespace Graph;

TEST(GraphIsomorphismTaggerSpectrumTest, CheckTagger)
{
    CompareSpectrumFactory factory;
    GraphTest::CheckComparerBasics(factory);
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
