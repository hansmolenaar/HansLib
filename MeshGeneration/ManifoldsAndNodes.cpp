#include "Defines.h"
#include "ManifoldsAndNodes.h"

using namespace MeshGeneration;
using namespace Geometry;

template ManifoldsAndNodes<GeomDim2>;
template ManifoldsAndNodes<GeomDim3>;

template<int N>
ManifoldsAndNodes<N>::ManifoldsWithNode ManifoldsAndNodes<N>::getManifoldsContainingNode(NodeIndex node) const
{
   const auto found = m_node2manifolds.find(node);
   if (found != m_node2manifolds.end())
   {
      return found->second;
   }
   return {};
}

template<int N>
void ManifoldsAndNodes<N>::deleteNode(NodeIndex node)
{
   const auto found = m_node2manifolds.find(node);
   if (found == m_node2manifolds.end())
   {
      throw MyException("ManifoldsAndNodes<N>::deleteNode unknown node specified");
   }
   for (const auto* m : found->second)
   {
      const auto numErased = m_manifold2node.at(m).erase(node);
      if (numErased != 1)
      {
         throw MyException("ManifoldsAndNodes<N>::deleteNode inconsistency");
      }
   }
   m_node2manifolds.erase(node);
}

template<int N>
void ManifoldsAndNodes<N>::addNodeToManifold(NodeIndex node, ManifoldPtrN manifoldPtr)
{
   m_node2manifolds[node].push_back(manifoldPtr);
   m_manifold2node[manifoldPtr].insert(node);
}

template<int N>
std::vector<NodeIndex> ManifoldsAndNodes<N>::getNodesInManifold(ManifoldPtrN manifoldPtr) const
{
   const auto found = m_manifold2node.find(manifoldPtr);
   if (found != m_manifold2node.end())
   {
      return { found->second.begin(), found->second.end() };
   }
   return {};
}