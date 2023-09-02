#include "IntervalTreeIndex1Factory.h"

using namespace IntervalTree;

Index1Factory::Index1Factory()
{
   const auto root = Index1::CreateRoot();
   add(Index1::CreateRoot());
}

const Index1* Index1Factory::getRoot() const
{
   return (*this)(0);
}

std::array<const Index1*, 2> Index1Factory::refine(const Index1& toRefine)
{
   std::array<Index1, 2> kids = toRefine.refine();
   add(kids[0]);
   add(kids[1]);
   return { (*this)(kids[0].getKey()), (*this)(kids[1].getKey()) };
}

const Index1* Index1Factory::operator()(Index1::Key key) const
{
   return  &m_cache.at(key);
}

Index1::Key Index1Factory::add(const Interval<Rational>& interval)
{
   return add(Index1(interval));
}

Index1::Key Index1Factory::add(const Index1& index1)
{
   const Index1::Key key = index1.getKey();
   if (!m_cache.contains(key))
   {
      m_cache.emplace(key, index1);
   }
   if (!m_toParent.contains(key))
   {
      if (index1.isRoot())
      {
         m_toParent[key] = Index1::KeyInvalid;
      }
      else
      {
         const auto parent = index1.getParent();
         // Interesting recursion here
         add(parent);
         m_toParent[key] = parent.getKey();
      }
   }
   return key;
}

const Index1* Index1Factory::getParent(Index1::Key key) const
{
   const Index1::Key parentKey = m_toParent.at(key);
   if (parentKey == Index1::KeyInvalid) return nullptr;
   return (*this)(parentKey);
}

const Index1* Index1Factory::getParent(const Index1& indx1) const
{
   return getParent(indx1.getKey());
}