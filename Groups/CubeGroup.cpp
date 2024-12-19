#include "CubeGroup.h"
#include "GroupTable.h"
#include "IndexerRowMajor.h"
#include "MyException.h"
#include "PermutationUtils.h"

namespace
{
   constexpr unsigned int nrDims = 3;
   constexpr unsigned int numRotations = 23;


   const std::array<std::array<bool, nrDims>, CubeGroup::numVertices> Coordinates
   {
      std::array<bool, nrDims>{false, false, false},
      std::array<bool, nrDims>{true, false, false},
      std::array<bool, nrDims>{false, true, false},
      std::array<bool, nrDims>{true, true, false},

      std::array<bool, nrDims>{false, false, true},
      std::array<bool, nrDims>{true, false, true},
      std::array<bool, nrDims>{false, true, true},
      std::array<bool, nrDims>{true, true, true},
   };

   std::vector<Permutation> Symmetries =
   {
      Permutation::CreateTrivial(CubeGroup::numVertices), // Identity

      Permutation::CreateFromDisjunctCycles(CubeGroup::numVertices, {{1, 3, 7, 5}, {0, 2, 6, 4}}),     // i axis 4-fold, pi/2
      Permutation::CreateFromDisjunctCycles(CubeGroup::numVertices, {{1, 7}, {3, 5}, {0, 6},{2, 4} }), // i axis 4-fold, pi
      Permutation::CreateFromDisjunctCycles(CubeGroup::numVertices, {{1,5, 7, 3}, {0, 4, 6, 2} }),     // i axis 4-fold, 3*pi/2

      Permutation::CreateFromDisjunctCycles(CubeGroup::numVertices, {{0, 1, 5, 4}, {2, 3, 7, 6}}),     // j axis 4-fold, pi/2
      Permutation::CreateFromDisjunctCycles(CubeGroup::numVertices, {{0, 5}, {1, 4}, {2, 7},{3, 6} }), // j axis 4-fold, pi
      Permutation::CreateFromDisjunctCycles(CubeGroup::numVertices, {{0, 4, 5, 1}, {2, 6, 7, 3} }),     // j axis 4-fold, 3*pi/2

      Permutation::CreateFromDisjunctCycles(CubeGroup::numVertices, {{0, 1, 3, 2}, {4, 5, 7, 6}}),     // j axis 4-fold, pi/2
      Permutation::CreateFromDisjunctCycles(CubeGroup::numVertices, {{0, 3}, {1, 2}, {4, 7},{5, 6} }), // j axis 4-fold, pi
      Permutation::CreateFromDisjunctCycles(CubeGroup::numVertices, {{0, 2, 3, 1}, {4, 6, 7, 5} }),    // j axis 4-fold, 3*pi/2

      Permutation::CreateFromDisjunctCycles(CubeGroup::numVertices, {{1, 4, 2}, {3, 5, 6}}),                  //  3-fold, 2*pi/3 0
      Permutation::CreateFromDisjunctCycles(CubeGroup::numVertices, {{1, 2, 4},{ 3, 6, 5} }),                  //  3-fold, 4*pi/3 0

      Permutation::CreateFromDisjunctCycles(CubeGroup::numVertices, {{0, 5, 3}, {2, 4, 7}}),                  //  3-fold, 2*pi/3 1
      Permutation::CreateFromDisjunctCycles(CubeGroup::numVertices, {{0, 3, 5},{ 2, 7, 4} }),                  //  3-fold, 4*pi/3 1

      Permutation::CreateFromDisjunctCycles(CubeGroup::numVertices, {{0, 3, 6}, {1, 7, 4}}),                  //  3-fold, 2*pi/3 2
      Permutation::CreateFromDisjunctCycles(CubeGroup::numVertices, {{0, 6, 3},{ 1, 4, 7} }),                  //  3-fold, 4*pi/3 2

      Permutation::CreateFromDisjunctCycles(CubeGroup::numVertices, {{0, 5, 6}, {1, 7, 2}}),                  //  3-fold, 2*pi/3 3
      Permutation::CreateFromDisjunctCycles(CubeGroup::numVertices, {{0, 6, 5},{ 1, 2, 7} }),                  //  3-fold, 4*pi/3 2

      Permutation::CreateFromDisjunctCycles(CubeGroup::numVertices, {{0, 1}, {6,7}, {2,5}, {3,4}}),                  //  2-fold,  edge 0-1
      Permutation::CreateFromDisjunctCycles(CubeGroup::numVertices, {{2, 3}, {4,5}, {0,7}, {1,6}}),                  //  2-fold,  edge 2-3

      Permutation::CreateFromDisjunctCycles(CubeGroup::numVertices, {{0, 2}, {5,7}, {1,6}, {3,4}}),                  //  2-fold,  edge 0-2
      Permutation::CreateFromDisjunctCycles(CubeGroup::numVertices, {{1, 3}, {4,6}, {0,7}, {2,5}}),                  //  2-fold,  edge 1-3

      Permutation::CreateFromDisjunctCycles(CubeGroup::numVertices, {{0, 4}, {3,7}, {1,6}, {2,5}}),                  //  2-fold,  edge 0-4
      Permutation::CreateFromDisjunctCycles(CubeGroup::numVertices, {{1, 5}, {2,6}, {0,7}, {3,4}}),                  //  2-fold,  edge 1-5

      Permutation::CreateFromDisjunctCycles(CubeGroup::numVertices, {{0, 1}, {2,3}, {4,5}, {6,7}}),                  //  Reflection i
      Permutation::CreateFromDisjunctCycles(CubeGroup::numVertices, {{0, 2}, {1,3}, {4,6}, {5,7}}),                  //  Reflection j
      Permutation::CreateFromDisjunctCycles(CubeGroup::numVertices, {{0, 4}, {1,5}, {3,7}, {2,6}}),                  //  Reflection k

      Permutation::CreateFromDisjunctCycles(CubeGroup::numVertices, {{2, 4}, {3,5}}),                                //  Reflection 0-1
      Permutation::CreateFromDisjunctCycles(CubeGroup::numVertices, {{1, 4}, {3,6}}),                                //  Reflection 0-2
      Permutation::CreateFromDisjunctCycles(CubeGroup::numVertices, {{0, 5}, {2,7}}),                                //  Reflection 1-3
      Permutation::CreateFromDisjunctCycles(CubeGroup::numVertices, {{0, 6}, {1,7}}),                                //  Reflection 2-3
      Permutation::CreateFromDisjunctCycles(CubeGroup::numVertices, {{1, 2}, {5,6}}),                                //  Reflection 0-4
      Permutation::CreateFromDisjunctCycles(CubeGroup::numVertices, {{0, 3}, {4,7}}),                                //  Reflection 1-5
   };

}

GroupElement CubeGroup::getOrder() const
{
   return 48;
}

GroupElement CubeGroup::getIdentity() const
{
   return 0;
}

GroupElement CubeGroup::getInverse(GroupElement) const
{
   throw MyException("CubeGroup::getInverse not yet implemented");
}

GroupElement CubeGroup::operator()(GroupElement, GroupElement) const
{
   throw MyException("CubeGroup::operator not yet implemented");
}

std::vector<Permutation> CubeGroup::getRotations()
{
   std::vector<Permutation> result;
   result.reserve(23);
   for (size_t n = 1; n < 24; ++n)
   {
      result.emplace_back(Symmetries[n]);
   }
   return result;
}

std::vector<Permutation> CubeGroup::getReflections()
{
   std::vector<Permutation> result;
   result.reserve(9);
   for (size_t n = 24; n < 33; ++n)
   {
      result.emplace_back(Symmetries[n]);
   }
   return result;
}

int CubeGroup::getDistance(Vertex v1, Vertex v2)
{
   int result = 0;
   const auto& v1c = Coordinates[v1];
   const auto& v2c = Coordinates[v2];
   for (auto n = 0; n < nrDims; ++n)
   {
      if (v1c[n] != v2c[n]) result += 1;
   }
   return result;
}

bool CubeGroup::isIsometry(const Permutation& permutation)
{
   constexpr std::array<Vertex, numVertices> ref = { 0,1,2,3,4,5,6,7 };
   Utilities::MyAssert(permutation.getCardinality() == numVertices);
   std::array<Vertex, numVertices> image;
   permutation.apply(ref.begin(), image.begin());
   for (auto v0 : ref)
   {
      for (auto v1 : ref)
      {
         if (v1 > v0)
         {
            if (CubeGroup::getDistance(v0, v1) != CubeGroup::getDistance(permutation(v0), permutation(v1))) return false;
         }
      }
   }

   return true;
}

std::vector<Permutation> CubeGroup::getGroupSymmetries()
{
   static std::vector<Permutation> s_permutations;
   if (s_permutations.empty())
   {
      const auto& reflection = Symmetries.at(numRotations + 1);
      s_permutations.reserve(48);
      std::copy_n(Symmetries.begin(), numRotations + 1, std::back_inserter(s_permutations));
      for (size_t n = 0; n < numRotations + 1; ++n)
      {
         s_permutations.emplace_back(Symmetries.at(n) * reflection);
      }
   }
   return s_permutations;
}

std::vector<Permutation> CubeGroup::getReflectionsIjk()
{
   static std::vector<Permutation> s_permutations;
   if (s_permutations.empty())
   {
      s_permutations.reserve(3);
      std::copy_n(Symmetries.begin() + numRotations + 1, 3, std::back_inserter(s_permutations));
   }
   return s_permutations;
}

std::vector<Permutation> CubeGroup::getSymmetries()
{
   return Symmetries;
}