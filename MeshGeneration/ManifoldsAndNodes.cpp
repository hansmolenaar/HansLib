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
   else
   {
      const auto otherManifolds = getManifoldsContainingNode(node);
      bool isAlsoInLower = false;
      for (const auto& m : otherManifolds)
      {
         if (m->getTopologyDimension() < manifoldPtr->getTopologyDimension())
         {
            isAlsoInLower = true;
            break;
         }
      }
      if (!isAlsoInLower)
      {
         const std::string msg = "Try to add node " + std::to_string(node) + " to manifold " + manifoldPtr->getName() + "; it is already in " + ManifoldsAndNodes::toString(otherManifolds);
         throw MyException(msg);
      }
      // add node to lower  dimensional manifold
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
   for (const auto* m : found->second)
   {
      if (manifoldPtr->getTopologyDimension() > m->getTopologyDimension())
      {
         return false;
      }
      else if (manifoldPtr->getTopologyDimension() == m->getTopologyDimension() && (*manifoldPtr != *m))
      {
         return false;
      }
   }
   return true;
}


template<int N>
std::string ManifoldsAndNodes<N>::toString(const ManifoldsWithNode& manifoldsWithNode)
{
   std::string result;
   bool first = true;
   for (const auto& m : manifoldsWithNode)
   {
      if (!first)
      {
         result += "    ";
      }
      first = false;
      result += m->getName();
   }
   return result;
}