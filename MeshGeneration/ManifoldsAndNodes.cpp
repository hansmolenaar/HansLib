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

   if (!m_node2manifolds.contains(node))
   {
      m_node2manifolds[node].push_back(manifoldPtr);
   }
   else if (std::find(m_node2manifolds[node].begin(), m_node2manifolds[node].end(), manifoldPtr) != m_node2manifolds[node].end())
   {
      // Don't add twice
      return;
   }

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

template<int N>
bool ManifoldsAndNodes<N>::isMobileOnManifold(NodeIndex node, ManifoldPtrN manifoldPtr) const
{
   const auto found = m_node2manifolds.find(node);
   if (found == m_node2manifolds.end()) return true;
   // Only movable if on the lowest dimensional manifold, e.g. the intersection line of 2 surfaces
   return str::none_of(found->second, [manifoldPtr](const auto* m) {return manifoldPtr->GetTopologyDimension() > m->GetTopologyDimension(); });
}