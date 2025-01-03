#pragma once

#include "IManifold.h"
#include "MeshGenerationDefines.h"
#include <vector>
#include <unordered_map>
#include <set>

namespace MeshGeneration
{
   template<int N>
   class ManifoldsAndNodes
   {
   public:
      using ManifoldPtrN = const Geometry::IManifold<GeomType, N>*;
      using ManifoldsWithNode = boost::container::static_vector<ManifoldPtrN, 3>;

      void deleteNode(NodeIndex node);
      void addNodeToManifold(NodeIndex node, ManifoldPtrN manifoldPtr);
      ManifoldsWithNode getManifoldsContainingNode(NodeIndex node) const;
      std::vector<NodeIndex> getNodesInManifold(ManifoldPtrN manifoldPtr) const;
      bool isMobileOnManifold(NodeIndex node, ManifoldPtrN manifoldPtr) const;
   private:
      // Use ordered set for test stability
      std::unordered_map<ManifoldPtrN, std::set<NodeIndex>> m_manifold2node;
      std::unordered_map<NodeIndex, ManifoldsWithNode> m_node2manifolds;
   };
}