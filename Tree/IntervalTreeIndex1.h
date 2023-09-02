#pragma once

#include "Rational.h"
#include "Interval.h"
#include "IntervalTreeDefines.h"
#include "Defines.h"

namespace IntervalTree
{

   class Index1
   {
   public:
      using Key = int;

      Index1(const Interval<Rational>& interval);
      Level getLevel() const;
      int getPositionInLevel() const;
      Key getKey() const;
      const Interval<Rational>& getInterval() const;
      std::array<Index1, 2> refine() const;
      Rational getMeasure() const;
      std::string toString() const;

   private:
      Interval<Rational> m_interval;
      Level m_level;
      int m_positionInLevel;
      Key m_key;
   };

}
