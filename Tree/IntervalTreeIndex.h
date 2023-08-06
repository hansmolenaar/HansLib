#pragma once

#include "Rational.h"
#include "Interval.h"
#include "IntervalTreeDefines.h"

#include <vector>
#include <unordered_map>

namespace IntervalTree
{
   using Index1Key = int;
   using FlyWeightKey = int;

   class Index1
   {
   public:
      Index1(const Interval<Rational>& interval);
      Level getLevel() const;
      int getPositionInLevel() const;
      FlyWeightKey getKey() const;
   private:
      Interval <Rational> m_interval;
      Level m_level;
      int m_positionInLevel;
      FlyWeightKey m_key;
   };

   class Index1FlyWeightFactory
   {
   public:
      const Index1& operator()(Index1Key key) const;
      Index1Key get(const Index1& index1);

   private:
      std::unordered_map<FlyWeightKey, Index1> m_cache;
   };

}