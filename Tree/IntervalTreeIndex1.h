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
      static constexpr Key KeyInvalid = std::numeric_limits<int>::lowest();

      static Index1 Create(const Interval<Rational>& interval);
      static Index1 CreateFromKey(Key key);
      static Index1 CreateRoot();

      auto operator<=>(const Index1&) const = default;

      Level getLevel() const;
      int getPositionInLevel() const;

      Key getKey() const;
      static std::tuple<Level, int> decomposeKey(int key);
      static int composeKey(Level level, int positionInLevel);
      const Interval<Rational>& getInterval() const;

      
      static std::array<Index1::Key, 2> refine(Index1::Key key);
      std::array<Index1, 2> refine() const;

      Rational getMeasure() const;
      std::string toString() const;
      bool isRoot() const;
      Rational getCenter() const;
      Index1 getSibling() const;
      std::tuple<bool, Index1> getSiblingInDir(bool posDir) const;
      std::tuple<bool, Index1> getAdjacentInDir(bool posDir) const;
      Index1 getParent() const;
      Index1::Key getParentKey() const;

   private:
      
      Index1(const Interval<Rational>& interval);

      Interval<Rational> m_interval;
      Level m_level;
      int m_positionInLevel;
      Key m_key;
   };

}
