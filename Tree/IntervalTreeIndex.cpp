#include "IntervalTreeIndex.h"

#include "MyException.h"
#include <cmath>

using namespace IntervalTree;

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