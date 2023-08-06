#include "IntervalTreeIndex.h"

#include "MyException.h"
#include <cmath>

using namespace IntervalTree;

namespace
{
   Level GetLevel(const Interval<Rational>& interval)
   {
      if (interval.getLower() < 0)
      {
         throw MyException("IntervalTreeIndex::GetLevel negative lowr bound");
      }

      const auto dif = interval.getUpper() - interval.getLower();
      if (dif.numerator() != 1)
      {
         throw MyException("IntervalTreeIndex::GetLevel interval not unit size");
      }

      const Level result = static_cast<Level>(std::round(std::log2(dif.denominator())));
      if (1 << result != dif.denominator())
      {
         throw MyException("GetLevel: incorrect interval");
      }
      return result;
   }

   int GetPositionInLevel(const Interval<Rational>& interval, Level level)
   {
      if (interval.getLower().denominator() >= interval.getUpper().denominator())
      {
         return interval.getLower().numerator();
      }
      return interval.getUpper().numerator() - 1;
   }

   FlyWeightKey GetKey(Level level, int positionInLevel)
   {
      const FlyWeightKey pow2m1 = (1 << level) - 1;
      return pow2m1 + positionInLevel;
   }
}

Index1::Index1(const Interval<Rational>& interval) :
   m_interval(interval),
   m_level(GetLevel(m_interval)),
   m_positionInLevel(GetPositionInLevel(m_interval, m_level)),
   m_key(GetKey(m_level, m_positionInLevel))
{
   // Final checks
   const auto denom = 1 << getLevel();
   if (m_interval.getLower() != Rational(getPositionInLevel(), denom))
   {
      throw MyException("Index1::Index1 incorrect interval, lower");
   }
   if (m_interval.getUpper() != Rational(getPositionInLevel()+1, denom))
   {
      throw MyException("Index1::Index1 incorrect interval, upper");
   }
}

Level Index1::getLevel() const
{
   return m_level;
}

int Index1::getPositionInLevel() const
{
   return m_positionInLevel;
}

FlyWeightKey Index1::getKey() const
{
   return m_key;
}

// !!!!!!!!!!!!!!!!!!!!! Factory

const Index1& Index1FlyWeightFactory::operator()(Index1Key key) const
{
   return  m_cache.at(key);
}

Index1Key Index1FlyWeightFactory::get(const Index1& index1)
{
   const FlyWeightKey key = index1.getKey();
   if (!m_cache.contains(key))
   {
      m_cache.emplace(key, index1);
   }
   return key;
}