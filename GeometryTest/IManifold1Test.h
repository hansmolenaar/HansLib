#pragma once

#include <gtest/gtest.h>
#include "IManifold1.h"

#include <algorithm> 
#include <cstdlib>
#include <random>

using namespace Geometry;

namespace
{
   template<typename T, int N>
   std::vector < Point<T, N>> GenerateSomePoints(int numPointToGenerate)
   {
      std::vector < Point<T, N>> result;
      std::vector<T> values(N * numPointToGenerate);
      for (int n = 0; n < N * numPointToGenerate; ++n)
      {
         values[n] = n + 1;
      }

      std::random_device rd;
      std::mt19937 g(rd());

      std::shuffle(values.begin(), values.end(), g);
      for (int n = 0; n < numPointToGenerate; ++n)
      {
         Point<T, N> point;
         for (int d = 0; d < N; ++d)
         {
            point[d] = values.at(n * N + d);
         }
         result.emplace_back(point);
      }
      return result;
   }
}

template<typename T, int N>
void IManifold1TestInterface(const IManifold1<T, N>& manifold, const IGeometryPredicate<T, N>& predicate)
{
   ASSERT_EQ(manifold.GetTopologyDimension(), Topology::Edge);
   const auto somePoints = GenerateSomePoints<T, N>(100);
   for (const auto& point : somePoints)
   {
      if (!manifold.contains(point, predicate))
      {
         ASSERT_THROW(manifold.GetEuclideanSubspaceAt(point, predicate), MyException);
         break;
      }
   }
}

