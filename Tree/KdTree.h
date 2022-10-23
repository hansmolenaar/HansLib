#pragma once

#include "Point/Point.h"

enum class KdTreeOverlap { NoOverlap, Overlap, Contains };

template<typename T, int N>
class IKdTreeTraversor;

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


};

template<typename T, int N>
class IKdTreeTraversor
{
public:
   virtual ~IKdTreeTraversor() = default;
   virtual void HandleLeaf(const KdTreeLeaf<T, N>&) = 0;
   virtual KdTreeOverlap DeterminOverlap(const Point<T, N>&, const Point<T, N>&) = 0;
};

template<typename T, int N>
class KdTree
{

};

template<typename T, int N>
class PreSorting
{
public:
   PreSorting(int , const std::span<const Point<T,N>>&);
   bool less(int, int) const;
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
      traversor.HandleLeaf(*dynamic_cast<const KdTreeLeaf<T, N>*>(this));
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
      traversor.HandleLeaf(*dynamic_cast<const KdTreeLeaf<T, N>*>(this));
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
PreSorting<T, N>::PreSorting(int skipDim, const std::span<const Point<T, N>>& points) : 
   m_currentDim(skipDim), m_points(points)
{
}

template<typename T, int N>
bool PreSorting<T, N>::less(int n0, int n1) const
{
   for (int n = 0; n < N; ++n)
   {
      if (n == m_currentDim) continue;
      if (m_points[n0][n] < m_points[n1][n]) return true;
   }
   return false;
}