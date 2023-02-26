#pragma once

#include "Defines.h"
#include "IKdTreeTraversor.h"
#include "Point.h"
#include "BoundingBox.h"
#include "KdTreeTraversorPointsInRange.h"

#include <memory>
#include <span>
#include <numeric>
#include <deque>
#include <limits>

template<typename T, int N>
class KdTreeVertex;

template<typename T, int N>
class KdTreeLeaf;


template<typename T, int N>
class KdTree
{
public:
   static std::unique_ptr< KdTree<T, N>> Create(const std::span<const Point<T, N>>&);
   std::vector<KdTreePosition> GetAllLeavesInOrder() const;
   Point<T, N> GetPoint(KdTreePosition) const;
   void Traverse(IKdTreeTraversor<T, N>& traversor);
   std::vector<KdTreePosition> FindInRange(const BoundingBox<T, N>&);

private:
   KdTree(std::span<const Point<T, N>> points);

   static KdTreeVertex<T, N>* BuildTree(std::array< std::span< KdTreePosition>, N>& orders, int depth, const std::span<const Point<T, N>>& points,
      std::vector< bool>& goRight, std::span<KdTreePosition> work, std::deque<KdTreeVertex<T, N>>& m_vertices);
   static std::vector<KdTreePosition> SortPerDimension(int thisDim, const std::span<const Point<T, N>> values);

   KdTreeVertex<T, N>* m_root = nullptr;
   std::span<const Point<T, N>> m_points;
   std::unique_ptr<BoundingBox<T, N>> m_bb;
   std::deque<KdTreeVertex<T, N>>  m_treeVertices;
};

// Forget the rest...

template<typename T, int N>
class KdTreeVertex
{
public:
   KdTreeVertex(KdTreeVertex<T, N>* left, KdTreeVertex<T, N>* right, T median, T nxt);
   KdTreeVertex(KdTreePosition pos);
   void HandleAllLeavesInSubTree(const KdTree<T, N>&, IKdTreeTraversor<T, N>&) const;
   void Traverse(const KdTree<T, N>&, IKdTreeTraversor<T, N>&, int level, Point<T, N>& lbound, Point<T, N>& ubound) const;
   bool isLeaf() const { return m_left == nullptr && m_right == nullptr; }
   KdTreePosition getPosition() const;

private:
   KdTreeVertex<T, N>* m_left = nullptr;
   KdTreeVertex<T, N>* m_right = nullptr;
   T                  m_median;
   T                  m_firstAfterMedian;
   KdTreePosition     m_position = KdTreePositionInvalid;
};

template<typename T, int N>
KdTreePosition  KdTreeVertex<T, N>::getPosition() const
{
   Utilities::MyAssert (isLeaf(), "KdTreeVertex<T, N>::getPosition() not a leaf");
   Utilities::MyAssert(m_position != KdTreePositionInvalid, "KdTreeVertex<T, N>::getPosition() position not set");
   return m_position;
}

template<typename T, int N>
class PreSorting
{
public:
   PreSorting(int, std::span<const Point<T, N>>);
   bool less(KdTreePosition, KdTreePosition) const;
private:
   int m_currentDim;
   std::span <const Point<T, N>> m_points;
};

namespace
{
   template<typename T, int N>
   void HandleLeaf(const KdTree<T, N>& tree, IKdTreeTraversor<T, N>& traversor, const KdTreeVertex<T, N>& vertex)
   {
      const auto pos = vertex.getPosition();
      const auto& point = tree.GetPoint(pos);
      traversor.HandleLeaf(pos, point);
   }
}

// KdTreeVertex
template<typename T, int N>
KdTreeVertex<T, N>::KdTreeVertex(KdTreeVertex<T, N>* left, KdTreeVertex<T, N>* right, T median, T nxt) :
   m_left(left), m_right(right), m_median(median), m_firstAfterMedian(nxt)
{}

template<typename T, int N>
KdTreeVertex<T, N>::KdTreeVertex(KdTreePosition pos) :
   m_position(pos)
{}


template<typename T, int N>
void KdTreeVertex<T, N>::HandleAllLeavesInSubTree(const KdTree<T, N>& tree, IKdTreeTraversor<T, N>& traversor) const
{
   if (isLeaf())
   {
      HandleLeaf(tree, traversor, *this);
   }
   else
   {
      if (m_left != nullptr)
      {
         m_left->HandleAllLeavesInSubTree(tree, traversor);
      }
      if (m_right != nullptr)
      {
         m_right->HandleAllLeavesInSubTree(tree, traversor);
      }
   }
}

template<typename T, int N>
void KdTreeVertex<T, N>::Traverse(const KdTree<T, N>& tree, IKdTreeTraversor<T, N>& traversor, int level, Point<T, N>& lbound, Point<T, N>& ubound) const
{
   if (isLeaf())
   {
      HandleLeaf(tree, traversor, *this);
   }
   else
   {
      const int dir = level % N;
      if (m_left != nullptr)
      {
         const T storeVal = ubound[dir];
         ubound[dir] = m_median;
         if (traversor.DeterminOverlap(lbound, ubound) != KdTreeOverlap::NoOverlap)
         {
            m_left->Traverse(tree, traversor, level + 1, lbound, ubound);
         }
         ubound[dir] = storeVal;
      }
      if (m_right != nullptr)
      {
         const T storeVal = lbound[dir];
         lbound[dir] = m_firstAfterMedian;
         if (traversor.DeterminOverlap(lbound, ubound) != KdTreeOverlap::NoOverlap)
         {
            m_right->Traverse(tree, traversor, level + 1, lbound, ubound);
         }
         lbound[dir] = storeVal;
      }
   }
}

template<typename T, int N>
std::vector<KdTreePosition> KdTree<T, N>::FindInRange(const BoundingBox<T, N>& searchRange)
{
   KdTreeTraversorPointsInRange<T, N> traversor(searchRange);
   Traverse(traversor);
   return traversor.GetFound();
}

// PreSorting

template<typename T, int N>
PreSorting<T, N>::PreSorting(int skipDim, std::span<const Point<T, N>> points) :
   m_currentDim(skipDim), m_points(points)
{
}

template<typename T, int N>
bool PreSorting<T, N>::less(KdTreePosition n0, KdTreePosition n1) const
{
   if (m_points[n0][m_currentDim] < m_points[n1][m_currentDim]) return true;
   if (m_points[n0][m_currentDim] > m_points[n1][m_currentDim]) return false;
   for (int n = 0; n < N; ++n)
   {
      if (m_points[n0][n] < m_points[n1][n]) return true;
   }
   return false;
}

// KdTree


template<typename T, int N>
std::vector<KdTreePosition> KdTree<T, N>::SortPerDimension(int thisDim, const std::span<const Point<T, N>> points)
{
   auto preSorting = std::make_unique< PreSorting<T, N>>(thisDim, points);
   const auto numPoints = points.size();
   std::vector<KdTreePosition> result(numPoints);
   std::iota(result.begin(), result.end(), static_cast<KdTreePosition>(0));
   std::sort(result.begin(), result.end(), [&preSorting](KdTreePosition n0, KdTreePosition n1) {return preSorting->less(n0, n1); });
   return result;
}

template<typename T, int N>
KdTree<T, N>::KdTree(std::span<const Point<T, N>> points) : m_points(points)
{
   if (!m_points.empty())
   {
      std::array<std::vector<KdTreePosition>, N>  orders;
      Point<T, N> lb, ub;
      for (int d = 0; d < N; ++d)
      {
         orders[d] = SortPerDimension(d, m_points);
         lb[d] = points[*orders[d].begin()][d];
         ub[d] = points[*orders[d].rbegin()][d];
      }
      m_bb = std::make_unique<BoundingBox<T, N>>(BoundingBox<T, N>::CreateFromList(std::array<Point<T, N>, 2 >{ lb, ub }));
      std::vector< bool> goRight(m_points.size(), false);
      std::array<  std::span<KdTreePosition>, N> ordersSpan;
      for (int n = 0; n < N; ++n)
      {
         ordersSpan[n] = orders[n];
      }
      std::vector<KdTreePosition> work(m_points.size());
      m_root = BuildTree(ordersSpan, 0, m_points, goRight, work, m_treeVertices);
   }
}

template<typename T, int N>
KdTreeVertex<T, N>* KdTree<T, N>::BuildTree(std::array< std::span< KdTreePosition>, N>& orders, int depth, const std::span<const Point<T, N>>& points,
   std::vector< bool>& goRight, std::span<KdTreePosition> work, std::deque<KdTreeVertex<T, N>>& buffer)
{
   const int activeDim = depth % N;
   const auto& order = orders[activeDim];
   if (order.size() == 1)
   {
      const auto pos = order[0];
      buffer.emplace_back(pos);
      return &buffer.back();
   }
   else
   {
      const auto numLeft = order.size() / 2;
      const auto numRight = order.size() - numLeft;

      std::array<std::span< KdTreePosition>, N> left;
      std::array<std::span< KdTreePosition>, N> right;


      left[activeDim] = std::span< KdTreePosition>(order.begin(), order.begin() + numLeft);
      right[activeDim] = std::span< KdTreePosition>(order.begin() + numLeft, order.begin() + numLeft + numRight);

      const T  median = points[order[numLeft - 1]][activeDim];
      const T nxt = points[order[numLeft]][activeDim];


      for (auto p : left[activeDim])
      {
         goRight[p] = false;
      }
      for (auto p : right[activeDim])
      {
         goRight[p] = true;
      }


      std::span<KdTreePosition> copyList(work.begin(), work.begin() + order.size());
      for (int d = 0; d < N; ++d)
      {
         if (d == activeDim) continue;

         auto& currentOrder = orders[d];
         std::copy(currentOrder.begin(), currentOrder.end(), copyList.begin());

         size_t p = 0;
         for (KdTreePosition pos : copyList)
         {
            if (goRight.at(pos)) continue;
            currentOrder[p] = pos;
            ++p;
         }

         for (KdTreePosition pos : copyList)
         {
            if (!goRight.at(pos)) continue;
            currentOrder[p] = pos;
            ++p;
         }

         left[d] = std::span<KdTreePosition>(currentOrder.begin(), currentOrder.begin() + numLeft);
         right[d] = std::span<KdTreePosition>(currentOrder.begin() + numLeft, currentOrder.begin() + numLeft + numRight);
      }


      auto* vertexLeft = BuildTree(left, depth + 1, points, goRight, work, buffer);
      auto* vertexRight = BuildTree(right, depth + 1, points, goRight, work, buffer);
      buffer.emplace_back(vertexLeft, vertexRight, median, nxt);
      return &buffer.back();
   }
}

template<typename T, int N>
std::unique_ptr< KdTree<T, N>> KdTree<T, N>::Create(const std::span<const Point<T, N>>& points)
{
   return std::unique_ptr< KdTree<T, N>>(new KdTree(points));
}

template<typename T, int N>
std::vector<KdTreePosition> KdTree<T, N>::GetAllLeavesInOrder() const
{
   if (m_root != nullptr)
   {
      KdTreeTraversorPointsInRange<T, N> traversor(*m_bb);
      m_root->HandleAllLeavesInSubTree(*this, traversor);
      return traversor.GetFound();
   }
   return {};
}

template<typename T, int N>
void KdTree<T, N>::Traverse(IKdTreeTraversor<T, N>& traversor)
{
   if (m_root != nullptr)
   {
      auto lwrBound = m_bb->getLower();
      auto uprBound = m_bb->getUpper();

      if (traversor.DeterminOverlap(lwrBound, uprBound) != KdTreeOverlap::NoOverlap)
      {
         m_root->Traverse(*this, traversor, 0, lwrBound, uprBound);
      }
   }
}

template<typename T, int N>
Point<T, N> KdTree<T, N>::GetPoint(KdTreePosition pos) const
{
   return m_points[pos];
}