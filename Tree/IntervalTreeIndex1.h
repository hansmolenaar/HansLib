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
      static Index1 CreateRoot();
      Level getLevel() const;
      int getPositionInLevel() const;
      Key getKey() const;
      const Interval<Rational>& getInterval() const;
      std::array<Index1, 2> refine() const;
      Rational getMeasure() const;
      std::string toString() const;
      bool isRoot() const;
      Rational getCenter() const;
      Index1 getSibling() const;
      std::tuple<bool, Index1> getSiblingInDir(bool posDir) const;

      std::tuple<bool, Index1> getAdjacentInDir(bool posDir) const;

   private:
      Interval<Rational> m_interval;
      Level m_level;
      int m_positionInLevel;
      Key m_key;
   };

}
