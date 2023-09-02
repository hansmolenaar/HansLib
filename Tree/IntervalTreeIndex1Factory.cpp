#include "IntervalTreeIndex1Factory.h"

using namespace IntervalTree;

Index1Factory::Index1Factory()
{
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
   return key;
}