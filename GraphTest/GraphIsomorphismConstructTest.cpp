#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismConstruct.h"
#include "IGraphIsomorphismTagger.h"
#include "Permutation.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace GraphIsomorphism;
using namespace Graph;

namespace
{
void CheckUniquenessGraphTaggers(const std::vector<std::unique_ptr<Graph::IGraphUs>> &graphs, int expectNumGraphs,
                                 int expectNumUniqueTags)
{
    const std::vector<IGraphTaggerFactory *> factories = Construct::getGraphTaggerFactories();
    ASSERT_EQ(graphs.size(), expectNumGraphs);
    std::set<std::vector<Tag>> uniqueTags;
    for (const auto &g : graphs)
    {
        ASSERT_TRUE(!g->isConnected());
        std::vector<Tag> tags;
        for (auto *f : factories)
        {
            tags.emplace_back(f->createGraphTagger(*g)->getGraphTag());
        }
        uniqueTags.insert(tags);
    }
    ASSERT_EQ(uniqueTags.size(), expectNumUniqueTags);
}

} // namespace

TEST(GraphIsomorphismConstructTest, DegreePath2and3)
{
    const auto graphs = std::make_pair(UndirectedGraphLibrary::Get_Path(2), UndirectedGraphLibrary::Get_Path(3));
    const auto status = Construct{}.actionConnected(*graphs.first, *graphs.second);
    ASSERT_EQ(status.getFlag(), Flag::NotIsomorphic);
}

TEST(GraphIsomorphismConstructTest, DegreePathAndStar4)
{
    const auto graphs =
        std::make_pair(UndirectedGraphLibrary::Get_Path(4), UndirectedGraphLibrary::Get_Star({1, 1, 1}));
    const auto status = Construct{}.actionConnected(*graphs.first, *graphs.second);
    ASSERT_EQ(status.getFlag(), Flag::NotIsomorphic);
}

TEST(GraphIsomorphismConstructTest, DegreePath3)
{
    const auto graphs = std::make_pair(UndirectedGraphLibrary::Get_Path(3), UndirectedGraphLibrary::Get_Path(3));
    const auto status = Construct{}.actionConnected(*graphs.first, *graphs.second);
    ASSERT_EQ(status.getFlag(), Flag::Isomorphic);
}

TEST(GraphIsomorphismConstructTest, DegreeStar123)
{
    const auto graphs =
        std::make_pair(UndirectedGraphLibrary::Get_Star({1, 2, 3}), UndirectedGraphLibrary::Get_Star({3, 1, 2}));
    const auto status = Construct{}.actionConnected(*graphs.first, *graphs.second);
    ASSERT_EQ(status.getFlag(), Flag::Isomorphic);
}

TEST(GraphIsomorphismConstructTest, DegreePan3)
{
    const auto graph = UndirectedGraphFromG6::CreateConnected(UndirectedGraphFromG6::pan3);
    const auto permuted = GraphUsc::CreatePermuted(*graph, {2, 1, 0, 3});
    const auto status = Construct{}.actionConnected(*graph, permuted);
    ASSERT_EQ(status.getFlag(), Flag::Isomorphic);
}

TEST(GraphIsomorphismConstructTest, Disconnected5)
{
    const auto graphs = UndirectedGraphFromG6::getDisconnectedGraphs(UndirectedGraphFromG6::getListNumVertices_5());
    CheckUniquenessGraphTaggers(graphs, 13, 13);
}

TEST(GraphIsomorphismConstructTest, Disconnected6)
{
    const auto graphs = UndirectedGraphFromG6::getDisconnectedGraphs(UndirectedGraphFromG6::getListNumVertices_6());
    CheckUniquenessGraphTaggers(graphs, 43, 38);
}

TEST(GraphIsomorphismConstructTest, Disconnected7)
{
    const auto graphs = UndirectedGraphFromG6::getDisconnectedGraphs(UndirectedGraphFromG6::getListNumVertices_7());
    CheckUniquenessGraphTaggers(graphs, 35, 30);
}

TEST(GraphIsomorphismConstructTest, Disconnected8)
{
    const auto graphs = UndirectedGraphFromG6::getDisconnectedGraphs(UndirectedGraphFromG6::getListNumVertices_8());
    CheckUniquenessGraphTaggers(graphs, 17, 16);
}

TEST(GraphIsomorphismConstructTest, Disconnected9)
{
    const auto graphs = UndirectedGraphFromG6::getDisconnectedGraphs(UndirectedGraphFromG6::getListNumVertices_9());
    CheckUniquenessGraphTaggers(graphs, 18, 16);
}
