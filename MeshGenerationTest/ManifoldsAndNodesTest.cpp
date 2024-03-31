#include <gtest/gtest.h>

#include "ManifoldsAndNodes.h"
#include "Manifold0.h"
#include "MeshGenerationDefines.h"
#include "Single.h"

using namespace MeshGeneration;
using namespace Geometry;
using namespace Utilities;


TEST(ManifoldsAndNodesTest, Empty)
{
   ManifoldsAndNodes<GeomDim2> man;
   const auto manifolds = man.getManifoldsContainingNode(1);
   ASSERT_TRUE(manifolds.empty());
   const auto nodes = man.getNodesInManifold(nullptr);
   ASSERT_TRUE(nodes.empty());

   ASSERT_THROW(man.deleteNode(1), MyException);
}


TEST(ManifoldsAndNodesTest, Single)
{
   const Point2 point{ 1,2 };
   const Manifold0<GeomType, GeomDim2> pointManifold(point);
   ManifoldsAndNodes<GeomDim2>::ManifoldPtrN manifoldPtr = &pointManifold;
   ManifoldsAndNodes<GeomDim2> manifoldsAndNodes;

   manifoldsAndNodes.addNodeToManifold(42, manifoldPtr);

   auto manifolds = manifoldsAndNodes.getManifoldsContainingNode(1);
   ASSERT_TRUE(manifolds.empty());
   manifolds = manifoldsAndNodes.getManifoldsContainingNode(42);
   ASSERT_EQ(Single(manifolds), manifoldPtr);

   const auto nodes = manifoldsAndNodes.getNodesInManifold(manifoldPtr);
   ASSERT_EQ(Single(nodes), 42);
}


TEST(ManifoldsAndNodesTest, Delete)
{
   const Point2 point{ 1,2 };
   const Manifold0<GeomType, GeomDim2> pointManifold(point);
   ManifoldsAndNodes<GeomDim2>::ManifoldPtrN manifoldPtr = &pointManifold;
   ManifoldsAndNodes<GeomDim2> manifoldsAndNodes;
   manifoldsAndNodes.addNodeToManifold(42, manifoldPtr);

   ASSERT_THROW(manifoldsAndNodes.deleteNode(1), MyException);
   auto nodes = manifoldsAndNodes.getNodesInManifold(manifoldPtr);
   ASSERT_EQ(Single(nodes), 42);
   auto manifolds = manifoldsAndNodes.getManifoldsContainingNode(42);
   ASSERT_FALSE(manifolds.empty());

   manifoldsAndNodes.deleteNode(42);
   nodes = manifoldsAndNodes.getNodesInManifold(manifoldPtr);
   ASSERT_TRUE(nodes.empty());
   manifolds = manifoldsAndNodes.getManifoldsContainingNode(42);
   ASSERT_TRUE(manifolds.empty());
}