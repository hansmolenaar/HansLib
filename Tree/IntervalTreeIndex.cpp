#include "IntervalTreeIndex.h"

#include "MyException.h"
#include <cmath>

using namespace IntervalTree;


// !!!!!!!!!!!!!!!!!!!!! Factory
Index1FlyWeightFactory::Index1FlyWeightFactory()
{
   add(Interval<Rational>({ 0,1 }, { 1,1 }));
}

const Index1* Index1FlyWeightFactory::getRoot() const
{
   return (*this)(0);
}

std::array<const Index1*, 2> Index1FlyWeightFactory::refine(const Index1& toRefine)
{
   std::array<Index1, 2> kids = toRefine.refine();
   add(kids[0]);
   add(kids[1]);
   return { (*this)(kids[0].getKey()), (*this)(kids[1].getKey()) };
}

const Index1* Index1FlyWeightFactory::operator()(Index1::Key key) const
{
   return  &m_cache.at(key);
}

Index1::Key Index1FlyWeightFactory::add(const Interval<Rational>& interval)
{
   return add(Index1(interval));
}

Index1::Key Index1FlyWeightFactory::add(const Index1& index1)
{
   const Index1::Key key = index1.getKey();
   if (!m_cache.contains(key))
   {
      m_cache.emplace(key, index1);
   }
   return key;
}

// !!!!!!!!!!!!! Index<>


template<>
std::array<std::array<Rational, 1>, 2> Index<1>::getVerticesInVtkOrder() const
{
   const auto& intv1 = getInterval(0);
   return { std::array<Rational, 1>{intv1.getLower()}, std::array<Rational, 1>{intv1.getUpper()} };
}


template<>
std::array<std::array<Rational, 2>, 4> Index<2>::getVerticesInVtkOrder() const
{
   const auto& intv0 = getInterval(0);
   const auto& intv1 = getInterval(1);
   return {
      std::array<Rational, 2>{intv0.getLower(), intv1.getLower()},
      std::array<Rational, 2>{intv0.getUpper(), intv1.getLower()},
      std::array<Rational, 2>{intv0.getUpper(), intv1.getUpper()},
      std::array<Rational, 2>{intv0.getLower(), intv1.getUpper()},
   };
}

template<>
std::array<std::array<Rational, 3>, 8> Index<3>::getVerticesInVtkOrder() const
{
   const auto& intvI = getInterval(0);
   const auto& intvJ = getInterval(1);
   const auto& intvK = getInterval(2);
   return {
      std::array<Rational, 3>{intvI.getLower(), intvJ.getLower(), intvK.getLower()},
      std::array<Rational, 3>{intvI.getUpper(), intvJ.getLower(), intvK.getLower()},
      std::array<Rational, 3>{intvI.getUpper(), intvJ.getUpper(), intvK.getLower()},
      std::array<Rational, 3>{intvI.getLower(), intvJ.getUpper(), intvK.getLower()},

      std::array<Rational, 3>{intvI.getLower(), intvJ.getLower(), intvK.getUpper()},
      std::array<Rational, 3>{intvI.getUpper(), intvJ.getLower(), intvK.getUpper()},
      std::array<Rational, 3>{intvI.getUpper(), intvJ.getUpper(), intvK.getUpper()},
      std::array<Rational, 3>{intvI.getLower(), intvJ.getUpper(), intvK.getUpper()},
   };
}