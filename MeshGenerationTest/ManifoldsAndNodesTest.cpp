#include <gtest/gtest.h>

#include "ManifoldsAndNodes.h"
#include "Manifold0.h"
#include "MeshGenerationDefines.h"
#include "Single.h"
#include "Sphere2AsManifold1.h"

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


TEST(ManifoldsAndNodesTest, AddTwice)
{
   const Point2 point{ 1,2 };
   const Manifold0<GeomType, GeomDim2> pointManifold(point);
   ManifoldsAndNodes<GeomDim2>::ManifoldPtrN manifoldPtr = &pointManifold;
   ManifoldsAndNodes<GeomDim2> manifoldsAndNodes;

   manifoldsAndNodes.addNodeToManifold(42, manifoldPtr);
   manifoldsAndNodes.addNodeToManifold(42, manifoldPtr);

   const auto manifolds = manifoldsAndNodes.getManifoldsContainingNode(42);
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


TEST(ManifoldsAndNodesTest, IsMobile)
{
   const Manifold0<GeomType, GeomDim2> pointManifold(Point2{ 1,2 });
   const Sphere2AsManifold1<GeomType> sphereManifold(Sphere<GeomType, GeomDim2>({0,0}, 1.0));

   ManifoldsAndNodes<GeomDim2>::ManifoldPtrN pointManifoldPtr = &pointManifold;
   ManifoldsAndNodes<GeomDim2>::ManifoldPtrN sphereManifoldPtr = &sphereManifold;

   ManifoldsAndNodes<GeomDim2> manifoldsAndNodes;

   const NodeIndex node{ 0 };
   ASSERT_TRUE(manifoldsAndNodes.isMobileOnManifold(node, pointManifoldPtr));

   manifoldsAndNodes.addNodeToManifold(node, pointManifoldPtr);
   ASSERT_TRUE(manifoldsAndNodes.isMobileOnManifold(node, pointManifoldPtr));
   ASSERT_FALSE(manifoldsAndNodes.isMobileOnManifold(node, sphereManifoldPtr));
}


TEST(ManifoldsAndNodesTest, IsMobileTwoCornerManifolds)
{
   const Manifold0<GeomType, GeomDim2> pointManifold1(Point2{ 1,2 });
   const Manifold0<GeomType, GeomDim2> pointManifold2(Point2{ 2,1 });

   ManifoldsAndNodes<GeomDim2>::ManifoldPtrN pointManifold1Ptr = &pointManifold1;
   ManifoldsAndNodes<GeomDim2>::ManifoldPtrN pointManifold2Ptr = &pointManifold2;

   ManifoldsAndNodes<GeomDim2> manifoldsAndNodes;

   const NodeIndex node{ 0 };
   ASSERT_TRUE(manifoldsAndNodes.isMobileOnManifold(node, pointManifold1Ptr));

   manifoldsAndNodes.addNodeToManifold(node, pointManifold1Ptr);
   ASSERT_TRUE(manifoldsAndNodes.isMobileOnManifold(node, pointManifold1Ptr));
   ASSERT_FALSE(manifoldsAndNodes.isMobileOnManifold(node, pointManifold2Ptr));
}