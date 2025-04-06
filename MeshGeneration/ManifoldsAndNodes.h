#pragma once

#include "IManifold.h"
#include "MeshGenerationDefines.h"
#include <ostream>
#include <set>
#include <unordered_map>
#include <vector>

namespace MeshGeneration
{
   template<int N>
   class ManifoldsAndNodes
   {
   public:
      using ManifoldPtrN = const Geometry::IManifold<GeomType, N>*;
      using ManifoldsWithNode = boost::container::static_vector<ManifoldPtrN, 3>;

      void deleteNode(Topology::NodeIndex node);
      void addNodeToManifold(Topology::NodeIndex node, ManifoldPtrN manifoldPtr);
      ManifoldsWithNode getManifoldsContainingNode(Topology::NodeIndex node) const;
      std::vector<Topology::NodeIndex> getNodesInManifold(ManifoldPtrN manifoldPtr) const;
      bool isMobileOnManifold(Topology::NodeIndex node, ManifoldPtrN manifoldPtr) const;

   private:
      // Use ordered set for test stability
      std::unordered_map<ManifoldPtrN, std::set<Topology::NodeIndex>> m_manifold2node;
      std::unordered_map<Topology::NodeIndex, ManifoldsWithNode> m_node2manifolds;
   };

}