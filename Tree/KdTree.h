#pragma once

#include "IKdTreeTraversor.h"
#include "Point/Point.h"
#include "BoundingBox/BoundingBox.h"

#include <memory>
#include <span>
#include <numeric>
#include <deque>
#include <limits>
#include <vector>

template<typename T, int N>
class KdTreeVertex
{
public:
   KdTreeVertex(KdTreeVertex<T, N>* left, KdTreeVertex<T, N>* right, T median, T nxt);
   void HandleAllLeavesInSubTree(IKdTreeTraversor<T, N>&) const;
   void Traverse(IKdTreeTraversor<T, N>&, int level, Point<T, N>& lbound, Point<T, N>& ubound) const;

private:
   bool isLeaf() const { return m_left == nullptr && m_right == nullptr; };
   KdTreeVertex<T, N>* m_left;
   KdTreeVertex<T, N>* m_right;
   T                  m_median;
   T                  m_firstAfterMedian;
};

template<typename T, int N>
class KdTreeLeaf : public KdTreeVertex<T, N>
{
public:
   KdTreeLeaf(KdTreeVertex<T, N>* left, KdTreeVertex<T, N>* right, T median, T nxt) : KdTreeVertex(left, right, median, nxt), m_position(std::numeric_limits<KdTreePosition>::max()) {}
   KdTreeLeaf(T value, KdTreePosition position) : KdTreeVertex(nullptr, nullptr, value, value), m_position(position) {}
private:
   KdTreePosition m_position;

};

template<typename T, int N>
class KdTree
{
public:
   static std::vector<KdTreePosition> SortPerDimension(int thisDim, const std::span<const Point<T, N>>& values);
   static KdTreeVertex<T, N>* BuildTree(std::array< std::span<const KdTreePosition>, N>& orders, int depth, const std::span<const Point<T, N>>& points,
      std::array< bool, N>& goRight, std::deque<KdTreeLeaf<T, N>>& m_vertices);
   static std::unique_ptr< KdTree<T, N>> Create(const std::span<const Point<T, N>>&);
   std::vector<KdTreePosition> GetAllLeavesInOrder() const;
   Point<T, N> GetPoint(KdTreePosition ) const;
   void Traverse(IKdTreeTraversor<T, N>& traversor);
private:
   KdTree(std::span<const Point<T, N>> points);
   KdTreeVertex<T, N>* m_root = nullptr;
   std::span <const Point<T, N>>& m_points;
   std::unique_ptr<BoundingBox<T, N>> m_bb;
   std::deque<KdTreeLeaf<T, N>>  m_vertices;
};

template<typename T, int N>
class PreSorting
{
public:
   PreSorting(int, std::span<const Point<T, N>>);
   bool less(KdTreePosition, KdTreePosition) const;
private:
   int m_currentDim;
   std::span <const Point<T, N>>& m_points;
};


// KdTreeVertex
template<typename T, int N>
KdTreeVertex<T, N>::KdTreeVertex(KdTreeVertex<T, N>* left, KdTreeVertex<T, N>* right, T median, T nxt) :
   m_left(left), m_right(right), m_median(median), m_firstAfterMedian(nxt)
{}


template<typename T, int N>
void KdTreeVertex<T, N>::HandleAllLeavesInSubTree(IKdTreeTraversor<T, N>& traversor) const
{
   if (isLeaf())
   {
      traversor.HandleLeaf(dynamic_cast<const KdTreeLeaf<T, N>*>(this)->getPosition());
   }
   else
   {
      if (m_left != nullptr)
      {
         m_left.GetAllLeavesInSubTree(traversor);
      }
      if (m_right != nullptr)
      {
         m_right.GetAllLeavesInSubTree(traversor);
      }
   }
}

template<typename T, int N>
void KdTreeVertex<T, N>::Traverse(IKdTreeTraversor<T, N>& traversor, int level, Point<T, N>& lbound, Point<T, N>& ubound) const
{
   if (isLeaf())
   {
      traversor.HandleLeaf(dynamic_cast<const KdTreeLeaf<T, N>*>(this)->getPosition());
   }
   else
   {
      const int dir = level % N;
      if (m_left != nullptr)
      {
         const T storeVal = ubound[dir];
         ubound[dir] = m_median;
         if (traversor.DeterminOverlap(lbound, ubound) != KdTreeOverlap.NoOverlap)
         {
            m_left.Traverse(traversor, level + 1, lbound, ubound);
         }
         ubound[dir] = storeVal;
      }
      if (m_right != nullptr)
      {
         const T storeVal = lbound[dir];
         lbound[dir] = m_firstAfterMedian;
         if (traversor.DeterminOverlap(lbound, ubound) != KdTreeOverlap.NoOverlap)
         {
            m_right.Traverse(traversor, level + 1, lbound, ubound);
         }
         lbound[dir] = storeVal;
      }
   }
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
   for (int n = 0; n < N; ++n)
   {
      if (n == m_currentDim) continue;
      if (m_points[n0][n] < m_points[n1][n]) return true;
   }
   return false;
}

// KdTree


template<typename T, int N>
std::vector<KdTreePosition> KdTree<T, N>::SortPerDimension(int thisDim, const std::span<const Point<T, N>>& points)
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
   }
}

template<typename T, int N>
KdTreeVertex<T, N>* KdTree<T, N>::BuildTree(std::array< std::span<const KdTreePosition>, N>& orders, int depth, const std::span<const Point<T, N>>& points,
   std::array< bool, N>& goRight, std::deque<KdTreeLeaf<T, N>>& buffer)
{
   const int activeDim = depth % N;
   const auto& order = orders[activeDim];
   if (order.size() == 1)
   {
      buffer.emplace_back(points[order[0]], 0);
      return &buffer[0];
   }
   else
   {
      const auto numLeft = order.size() / 2;
      const auto numRight = order.szie() - numLeft;

      std::array<std::span<KdTreePosition>, N> left;
      std::array<std::span<KdTreePosition>, N> right;


      left[activeDim] = std::span<KdTreePosition>(order.begin(), order.begin() + numLeft);
      right[activeDim] = std::span<KdTreePosition>(order.begin() + numLeft, order.begin() + numLeft + numRight);

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


      std::vector<KdTreePosition> copyList(order.size());
      for (int d = 0; d < N; ++d)
      {
         if (d == activeDim) continue;

         auto& currentOrder = orders[d];
         std::copy(copyList.begin(), copyList.end(), currentOrder.begin());

         size_t p = 0;
         for (KdTreePosition pos : copyList)
         {
            if (goRight(pos)) continue;
            currentOrder[p] = pos;
            ++p;
         }

         for (KdTreePosition pos : copyList)
         {
            if (!goRight(pos)) continue;
            currentOrder[p] = pos;
            ++p;
         }

         left[d] = std::span<KdTreePosition>(currentOrder.begin(), currentOrder.begin() + numLeft);
         right[d] = std::span<KdTreePosition>(currentOrder.begin() + numLeft, currentOrder.begin() + numLeft + numRight);
      }


      auto* vertexLeft = BuildTree(left, depth + 1, points, goRight, buffer);
      auto* vertexRight = BuildTree(right, depth + 1, points, goRight, buffer);
      buffer.emplace_back(vertexLeft, vertexRight, median, nxt);
      return &buffer.last();
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
     // auto traversor = KdTreeTraversorPointInRange<NumericType>.Create(BoundingBox, this);
    //  Root.GetAllLeavesInSubTree(traversor);
    //  return traversor.Found;
      throw std::exception("not yet implemented");
   }
   return {};
}

template<typename T, int N>
void KdTree<T, N>::Traverse(IKdTreeTraversor<T, N>& traversor)
{
   if (m_root != nullptr)
   {
#if false
         var lwrBound = BoundingBox.GetLowerAll().ToArray();
         var uprBound = BoundingBox.GetUpperAll().ToArray();
         if (traversor.DeterminOverlap(lwrBound, uprBound) != KdTreeOverlap.NoOverlap)
         {
            Root.Traverse(traversor, 0, BoundingBox.GetLowerAll().ToArray(), BoundingBox.GetUpperAll().ToArray());
         }
#else
      MessageHandler::Error("KdTree<T, N>::Traverse Not yet implemented");
#endif
   }
}

template<typename T, int N>
Point<T, N> KdTree<T, N>:: GetPoint(KdTreePosition pos) const
{
   return m_points[pos];
}