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
   const auto kid0 = addIfNew(kids[0].getKey());
   const auto kid1 = addIfNew(kids[1].getKey());
   return { kid0, kid1 };
}

const Index1* Index1Factory::operator()(Index1::Key key) const
{
   return  &m_cache.at(key);
}

const Index1* Index1Factory::add(const Interval<Rational>& interval)
{
   return addIfNew(Index1::Create(interval).getKey());
}

Index1::Key Index1Factory::add(const Index1& index1)
{
   return addIfNew(index1.getKey())->getKey();
}

const Index1* Index1Factory::addIfNew(Index1::Key key)
{
   if (!m_cache.contains(key))
   {
      m_cache.emplace(key, Index1::CreateFromKey(key));
   }
   const Index1* result = &m_cache.at(key);
   if (!m_toParent.contains(key))
   {
      if (result->isRoot())
      {
         m_toParent[key] = Index1::KeyInvalid;
      }
      else
      {
         const auto parentKey = result->getParentKey();
         // Interesting recursion here
         addIfNew(parentKey);
         m_toParent[key] = parentKey;
      }
   }
   return result;
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