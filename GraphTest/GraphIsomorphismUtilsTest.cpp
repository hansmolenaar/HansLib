#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismTaggerDegree.h"
#include "GraphIsomorphismUtils.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

#include <sstream>

using namespace Graph;
using namespace GraphIsomorphism;

TEST(GraphIsomorphismUtilsTest, CondenseSizeSequence_int)
{
    const std::vector<int> sizes{2, 5, 2};
    const auto tag = CondenseSizeSequence(sizes);
    ASSERT_TRUE(str::equal(tag, (Tag{2, 2, 5, 1})));
}

TEST(GraphIsomorphismUtilsTest, CondenseSizeSequence_size_t)
{
    const std::vector<size_t> sizes{2, 5, 2};
    const auto tag = CondenseSizeSequence(sizes);
    ASSERT_TRUE(str::equal(tag, (Tag{2, 2, 5, 1})));
}

TEST(GraphIsomorphismUtilsTest, StreamVertexGrouping)
{
    const auto graph = UndirectedGraphLibrary::Get_Claw();
    const TaggerDegree tagger(*graph);
    std::stringstream os;
    os << tagger.getVertexGrouping();
    ASSERT_EQ(os.str(), "Vertex group sizes : {1, 3}");
}
