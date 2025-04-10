#include "Permutation.h"
#include "TetrahedronNodesOriented.h"
#include <gtest/gtest.h>

using namespace Topology;

TEST(TetrahedronNodesOrientedTest, constructor)
{
   const TetrahedronNodesOriented tet{ 1, 2, 3, 4 };
   ASSERT_TRUE(tet.contains(1));
   ASSERT_TRUE(tet.contains(2));
   ASSERT_TRUE(tet.contains(3));
   ASSERT_TRUE(tet.contains(4));
   ASSERT_FALSE(tet.contains(5));
   ASSERT_FALSE(tet.contains(NodeIndexInvalid));
}

TEST(TetrahedronNodesOrientedTest, StreamInsertion)
{
   std::ostringstream os;
   const TetrahedronNodesOriented tet{ 2,1,5, 13 };
   os << tet;
   ASSERT_EQ(os.str(), "(1, 5, 2, 13)");
}

TEST(TetrahedronNodesOrientedTest, Permutations)
{
   int count = 0;
   const std::array<NodeIndex, NumNodesOnTetrahedron> trivial{ 1,2,3,4 };
   const std::array<NodeIndex, NumNodesOnTetrahedron> expectEven{ 1,2,3,4 };
   const std::array<NodeIndex, NumNodesOnTetrahedron> expectOdd{ 1,3,2,4 };
   std::vector<Permutation::Entry> p{ 0,1,2,3 };
   do
   {
      ++count;
      const Permutation permut = Permutation::Create(p);
      const auto parity = permut.getParity();

      auto tetNodes = trivial;
      permut.apply(trivial.begin(), tetNodes.begin());

      const TetrahedronNodesOriented tet(tetNodes);
      if (parity == Permutation::Parity::EVEN)
      {
         ASSERT_TRUE(str::equal(tet, expectEven));
      }
      else
      {
         ASSERT_TRUE(str::equal(tet, expectOdd));
      }
   } while (std::next_permutation(p.begin(), p.end()));
   ASSERT_EQ(count, 24);
}

TEST(TetrahedronNodesOrientedTest, Find)
{
   const TetrahedronNodesOriented tet{ 2, 1, 5, 13 };
   ASSERT_EQ(tet.find(2), 2);
   ASSERT_EQ(tet.find(1), 0);
   ASSERT_EQ(tet.find(5), 1);
   ASSERT_EQ(tet.find(13), 3);
   ASSERT_EQ(tet.find(3), NodeIndexInvalid);
}

TEST(TetrahedronNodesOrientedTest, Equals)
{
   const TetrahedronNodesOriented tet1{ 1, 2, 3, 4 };
   const TetrahedronNodesOriented tet2{ 4, 3, 2, 1 };
   ASSERT_TRUE(tet1 == tet2);
   ASSERT_FALSE(tet1 < tet2);
   ASSERT_FALSE(tet1 > tet2);
}
