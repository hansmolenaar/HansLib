#include <gtest/gtest.h>

#include "Defines.h"
#include "GraphIsomorphismTaggerKnown.h"
#include "IGraphUs.h"
#include "Single.h"
#include "UndirectedGraphFromG6.h"
#include "UndirectedGraphLibrary.h"

using namespace Utilities;
using namespace Graph;
using namespace GraphIsomorphism;

namespace
{
void CheckSequenceAfterSorting(std::span<const Vertex> actual, std::span<const Vertex> expectSorted)
{
    std::vector<Vertex> actualSorted(actual.begin(), actual.end());
    str::sort(actualSorted);
    if (!std::equal(actualSorted.begin(), actualSorted.end(), expectSorted.begin(), expectSorted.end()))
    {
        int hello = 1;
    }
    ASSERT_TRUE(std::equal(actualSorted.begin(), actualSorted.end(), expectSorted.begin(), expectSorted.end()));
}

void CheckDegreeSequence(const Graph::IGraphUs &graph, Vertex numVertices, std::span<const Vertex> degreeSequenceSorted)
{
    ASSERT_EQ(graph.getNumVertices(), numVertices);
    ASSERT_EQ(degreeSequenceSorted.size(), numVertices);
    const auto degrees = graph.getSortedDegreeSequence();
    CheckSequenceAfterSorting(degrees, degreeSequenceSorted);
}

std::vector<Vertex> GetCyclePathSize(const std::vector<std::vector<Vertex>> &cyclesPaths)
{
    std::vector<Vertex> result;
    for (const auto &cp : cyclesPaths)
    {
        result.push_back(cp.size());
    }
    return result;
}

void CheckCyclesPaths(const UndirectedGraph &graph, std::span<const Vertex> cycleSizesExpect,
                      std::span<const Vertex> pathSizesExpect)
{
    std::vector<std::vector<Vertex>> cycles;
    std::vector<std::vector<Vertex>> paths;

    const UndirectedGraph::CyclesAndPaths cyclesAndPaths = graph.SplitInCyclesAndPaths();
    CheckSequenceAfterSorting(GetCyclePathSize(cyclesAndPaths.Cycles), cycleSizesExpect);
    CheckSequenceAfterSorting(GetCyclePathSize(cyclesAndPaths.Paths), pathSizesExpect);
}
} // namespace
TEST(UndirectedGraphTest, ConnectedComponents)
{
    UndirectedGraph ug(3);

    ASSERT_EQ(ug.getNumVertices(), 3);
    ASSERT_EQ(ug.getNumEdges(), 0);

    constexpr Vertex vertex2 = 2;
    ug.addEdge(1, vertex2);
    ASSERT_EQ(ug.getNumEdges(), 1);
    ASSERT_EQ(ug.getDegree(0), 0);
    ASSERT_EQ(ug.getDegree(1), 1);
    ASSERT_EQ(ug.getDegree(2), 1);

    const auto components = ug.getConnectedComponents();
    ASSERT_EQ(*str::max_element(components), 1);
    ASSERT_EQ(components[vertex2], components[1]);
    ASSERT_NE(components[0], components[1]);
}

TEST(UndirectedGraphTest, Adjacency)
{
    UndirectedGraph ug(3);
    constexpr Vertex vertex2 = 2;
    ug.addEdge(1, vertex2);

    std::vector<Vertex> neighbors;
    ug.setAdjacentVertices(1, neighbors);
    ASSERT_EQ(Single(neighbors), 2);

    ug.setAdjacentVertices(0, neighbors);
    ASSERT_TRUE(neighbors.empty());

    ug.setAdjacentVertices(2, neighbors);
    ASSERT_EQ(Single(neighbors), 1);
}

TEST(UndirectedGraphTest, AdjacencyPath)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(3);
    std::vector<Vertex> neighbors;

    graph->setAdjacentVertices(0, neighbors);
    ASSERT_EQ(Single(neighbors), 1);

    graph->setAdjacentVertices(1, neighbors);
    ASSERT_EQ(neighbors.size(), 2);

    graph->setAdjacentVertices(2, neighbors);
    ASSERT_EQ(Single(neighbors), 1);
}

TEST(UndirectedGraphTest, GetCycle)
{
    constexpr size_t maxNumVertices = 10;
    for (size_t numVertices = 3; numVertices < maxNumVertices; ++numVertices)
    {
        const auto graph = UndirectedGraphLibrary::Get_Cycle(numVertices);
        ASSERT_EQ(graph->getNumVertices(), numVertices);
        ASSERT_EQ(graph->getNumEdges(), numVertices);
        for (Vertex v = 0; v < numVertices; ++v)
        {
            ASSERT_EQ(graph->getDegree(v), 2);
        }
    }
}

TEST(UndirectedGraphTest, GetCompleteGraph)
{
    constexpr size_t maxNumVertices = 6;
    for (size_t numVertices = 1; numVertices < maxNumVertices; ++numVertices)
    {
        const auto graph = UndirectedGraphLibrary::Get_CompleteGraph(numVertices);
        ASSERT_EQ(graph->getNumVertices(), numVertices);
        ASSERT_EQ(graph->getNumEdges(), numVertices * (numVertices - 1) / 2);
        for (Vertex v = 0; v < numVertices; ++v)
        {
            ASSERT_EQ(graph->getDegree(v), numVertices - 1);
        }
    }
}

TEST(UndirectedGraphTest, GetDiamond)
{
    const auto graph = UndirectedGraphLibrary::Get_Diamond();
    CheckDegreeSequence(*graph, 4, std::vector<Vertex>{2, 2, 3, 3});
}

TEST(UndirectedGraphTest, GetClaw)
{
    const auto graph = UndirectedGraphLibrary::Get_Claw();
    CheckDegreeSequence(*graph, 4, std::vector<Vertex>{1, 1, 1, 3});
}

TEST(UndirectedGraphTest, GetBull)
{
    const auto graph = UndirectedGraphLibrary::Get_Bull();
    CheckDegreeSequence(*graph, 5, std::vector<Vertex>{1, 1, 2, 3, 3});
}

TEST(UndirectedGraphTest, SplitInCyclesAndPaths)
{
    std::vector<std::vector<Vertex>> cycles;
    std::vector<std::vector<Vertex>> paths;

    UndirectedGraph graph(*UndirectedGraphLibrary::Get_Path(2));
    CheckCyclesPaths(graph, std::vector<Vertex>{}, std::vector<Vertex>{2});

    graph = UndirectedGraph(*UndirectedGraphLibrary::Get_Path(3));
    CheckCyclesPaths(graph, std::vector<Vertex>{}, std::vector<Vertex>{3});

    graph = UndirectedGraph(*UndirectedGraphLibrary::Get_Cycle(3));
    CheckCyclesPaths(graph, std::vector<Vertex>{3}, std::vector<Vertex>{});

    graph = UndirectedGraph(*UndirectedGraphLibrary::Get_Cycle(4));
    CheckCyclesPaths(graph, std::vector<Vertex>{4}, std::vector<Vertex>{});

    graph = UndirectedGraph(*UndirectedGraphLibrary::Get_Claw());
    CheckCyclesPaths(graph, std::vector<Vertex>{}, std::vector<Vertex>{2, 2, 2});

    graph = UndirectedGraph(*UndirectedGraphLibrary::Get_Diamond());
    CheckCyclesPaths(graph, std::vector<Vertex>{}, std::vector<Vertex>{2, 3, 3});

    graph = UndirectedGraph(*UndirectedGraphLibrary::Get_Bull());
    CheckCyclesPaths(graph, std::vector<Vertex>{}, std::vector<Vertex>{2, 2, 2, 3});

    graph = UndirectedGraph(*UndirectedGraphLibrary::Get_Paw());
    CheckCyclesPaths(graph, std::vector<Vertex>{3}, std::vector<Vertex>{2});

    graph = UndirectedGraph(*UndirectedGraphLibrary::Get_Butterfly());
    CheckCyclesPaths(graph, std::vector<Vertex>{3, 3}, std::vector<Vertex>{});

    for (Vertex numVertices = 0; numVertices < 4; ++numVertices)
    {
        graph = UndirectedGraph(*UndirectedGraphLibrary::Get_DisconnectedGraph(numVertices));
        CheckCyclesPaths(graph, std::vector<Vertex>{}, std::vector<Vertex>{});
    }

    graph = UndirectedGraph(*UndirectedGraphLibrary::Get_CompleteBipartite(3, 3));
    CheckCyclesPaths(graph, std::vector<Vertex>{}, std::vector<Vertex>(9, 2));
}

TEST(UndirectedGraphTest, SplitInCyclesAndPathsPan3)
{
    const auto graph = UndirectedGraphFromG6::Create(UndirectedGraphFromG6::pan3);
    const auto cap = graph->SplitInCyclesAndPaths();
    ASSERT_TRUE(str::equal(Single(cap.Cycles), std::vector<Vertex>{2, 0, 1}));
    ASSERT_TRUE(str::equal(Single(cap.Paths), std::vector<Vertex>{2, 3}));
    ASSERT_EQ(graph->getDegree(0), 2);
    ASSERT_EQ(graph->getDegree(1), 2);
    ASSERT_EQ(graph->getDegree(2), 3);
    ASSERT_EQ(graph->getDegree(3), 1);
}

TEST(UndirectedGraphTest, AddEdgeDuplicate)
{
    UndirectedGraph ug(2);
    ug.addEdge(0, 1);
    ug.addEdge(1, 0);
    ug.addEdge(0, 1);
    ug.addEdge(1, 0);

    ASSERT_EQ(ug.getNumEdges(), 1);
}

TEST(UndirectedGraphTest, AreAdjacent)
{
    UndirectedGraph ug(3);
    ug.addEdge(0, 1);
    ASSERT_TRUE(ug.areAdjacent(0, 1));
    ASSERT_TRUE(ug.areAdjacent(1, 0));
    ASSERT_FALSE(ug.areAdjacent(1, 2));
    ASSERT_ANY_THROW(ug.areAdjacent(1, 3));
}

TEST(UndirectedGraphTest, IsolatedVertices)
{
    UndirectedGraph ug(3);
    ug.addEdge(0, 2);
    auto isolated = ug.getIsolatedVertices();
    ASSERT_EQ(Single(isolated), 1);
    ug.addEdge(1, 2);
    isolated = ug.getIsolatedVertices();
    ASSERT_TRUE(isolated.empty());
}

TEST(UndirectedGraphTest, Star111)
{
    const auto graph = UndirectedGraphLibrary::Get_Star({1, 1, 2});
    ASSERT_EQ(graph->getNumVertices(), 5);
    ASSERT_EQ(graph->getNumEdges(), 4);
    const auto components = graph->getConnectedComponents();
    ASSERT_EQ(*str::max_element(components), 0);
}

TEST(UndirectedGraphTest, Star123)
{
    const auto graph = UndirectedGraphLibrary::Get_Star({1, 2, 3});
    ASSERT_EQ(graph->getNumVertices(), 7);
    ASSERT_EQ(graph->getNumEdges(), 6);
    const auto components = graph->getConnectedComponents();
    ASSERT_EQ(*str::max_element(components), 0);
}

TEST(UndirectedGraphTest, Permut)
{
    const auto graph = UndirectedGraphLibrary::Get_Star({1, 2, 3});
    const auto permutation = Permutation::Create(std::vector<Permutation::Entry>{1, 2, 3, 4, 5, 6, 0});
    const auto permuted = UndirectedGraph::CreatePermuted(*graph, permutation);
    ASSERT_EQ(permuted.getNumVertices(), 7);
    ASSERT_EQ(permuted.getNumEdges(), 6);
    const auto components = permuted.getConnectedComponents();
    ASSERT_EQ(*str::max_element(components), 0);

    std::vector<Vertex> degreeSequenceOrg = graph->getSortedDegreeSequence();

    std::vector<Vertex> degreeSequencePrm = permuted.getSortedDegreeSequence();

    ASSERT_TRUE(str::equal(degreeSequenceOrg, degreeSequencePrm));
}

TEST(UndirectedGraphTest, Complement_Null)
{
    const auto graph = UndirectedGraphLibrary::Get_Null();
    ASSERT_ANY_THROW(UndirectedGraph::CreateComplement(*graph));
}

TEST(UndirectedGraphTest, Complement_Singleton)
{
    const auto graph = UndirectedGraphLibrary::Get_Singleton();
    ASSERT_ANY_THROW(UndirectedGraph::CreateComplement(*graph));
}

TEST(UndirectedGraphTest, Complement_Path2)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(2);
    const auto complement = UndirectedGraph::CreateComplement(*graph);
    ASSERT_EQ(complement.getNumVertices(), 2);
    ASSERT_EQ(complement.getNumEdges(), 0);
}

TEST(UndirectedGraphTest, Complement_Cycle3)
{
    const auto graph = UndirectedGraphLibrary::Get_Cycle(3);
    const auto complement = UndirectedGraph::CreateComplement(*graph);
    ASSERT_EQ(complement.getNumVertices(), 3);
    ASSERT_EQ(complement.getNumEdges(), 0);
}

TEST(UndirectedGraphTest, Complement_Path3)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(3);
    const auto complement = UndirectedGraph::CreateComplement(*graph);
    ASSERT_EQ(complement.getNumVertices(), 3);
    ASSERT_EQ(complement.getNumEdges(), 1);
    ASSERT_TRUE(complement.areAdjacent(0, 2));
}

TEST(UndirectedGraphTest, Complement_path4)
{
    const auto graph = UndirectedGraphLibrary::Get_Path(4);
    const auto complement = UndirectedGraph::CreateComplement(*graph);
    GraphIsomorphism::TaggerKnown tagger(complement);
    ASSERT_EQ(tagger.getGraphTag(), (Tag{TaggerKnown::KnownType::Path, 4}));
}

TEST(UndirectedGraphTest, Complement_dianmond)
{
    const auto graph = UndirectedGraphLibrary::Get_Diamond();
    const auto complement = UndirectedGraph::CreateComplement(*graph);
    ASSERT_EQ(complement.getNumVertices(), 4);
    ASSERT_EQ(complement.getNumEdges(), 1);
    ASSERT_TRUE(complement.areAdjacent(0, 3));
}

TEST(UndirectedGraphTest, Complement_pan3)
{
    const auto graph = UndirectedGraphFromG6::Create(UndirectedGraphFromG6::pan3);
    const auto complement = UndirectedGraph::CreateComplement(*graph);
    ASSERT_EQ(complement.getNumVertices(), 4);
    ASSERT_EQ(complement.getNumEdges(), 2);
    ASSERT_TRUE(complement.areAdjacent(1, 3));
    ASSERT_TRUE(complement.areAdjacent(0, 3));
}

TEST(UndirectedGraphTest, Complement_house)
{
    const auto graph = UndirectedGraphFromG6::Create(UndirectedGraphFromG6::house);
    const auto complement = UndirectedGraph::CreateComplement(*graph);
    GraphIsomorphism::TaggerKnown tagger(complement);
    ASSERT_EQ(tagger.getGraphTag(), (Tag{TaggerKnown::KnownType::Path, 5}));
}

TEST(UndirectedGraphTest, Complement_disconnected5)
{
    const auto graph = UndirectedGraphLibrary::Get_DisconnectedGraph(5);
    const auto complement = UndirectedGraph::CreateComplement(*graph);
    GraphIsomorphism::TaggerKnown tagger(complement);
    ASSERT_EQ(tagger.getGraphTag(), (Tag{TaggerKnown::KnownType::Complete, 5}));
}

TEST(UndirectedGraphTest, CreateEdgesOmmitted_basic)
{
    const auto graph = UndirectedGraphLibrary::Get_Cycle(3);
    const std::vector<Vertex> omitBetween{1, 2, 1};
    const auto reduced = UndirectedGraph::CreateEdgesOmitted(*graph, std::vector<std::vector<Vertex>>{omitBetween});
    ASSERT_EQ(reduced.getNumVertices(), 3);
    ASSERT_EQ(reduced.getNumEdges(), 2);
    std::vector<Vertex> neighbors;
    reduced.setAdjacentVertices(1, neighbors);
    ASSERT_EQ(Single(neighbors), 0);
    reduced.setAdjacentVertices(2, neighbors);
    ASSERT_EQ(Single(neighbors), 0);
}

TEST(UndirectedGraphTest, CreateEdgesOmmitted_error)
{
    const auto graph = UndirectedGraphLibrary::Get_Cycle(3);
    const std::vector<Vertex> omitBetween{1, 2, 1};
    ASSERT_THROW(UndirectedGraph::CreateEdgesOmitted(
                     *graph, std::vector<std::vector<Vertex>>{std::vector<Vertex>{0, 1}, std::vector<Vertex>{1, 2}}),
                 MyException);
}
