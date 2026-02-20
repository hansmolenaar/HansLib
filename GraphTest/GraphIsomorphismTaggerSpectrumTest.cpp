#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismIGraphCompareTest.h"
#include "GraphIsomorphismTaggerSpectrum.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

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
