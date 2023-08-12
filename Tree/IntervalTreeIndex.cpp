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

   Index1::Key GetKey(Level level, int positionInLevel)
   {
      const Index1::Key pow2m1 = (1 << level) - 1;
      return pow2m1 + positionInLevel;
   }

   std::string ToString(const Rational& rat)
   {
      if (rat == Rational(0, 1)) return "0";
      if (rat == Rational(1, 1)) return "1";
      return std::to_string(rat.numerator()) + "/" + std::to_string(rat.denominator());
   }

   std::string ToString(const Interval<Rational>& intv)
   {
      return "(" + ToString(intv.getLower()) + ", " + ToString(intv.getUpper()) + ")";
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
   if (m_interval.getUpper() != Rational(getPositionInLevel() + 1, denom))
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

Index1::Key Index1::getKey() const
{
   return m_key;
}

const Interval<Rational>& Index1::getInterval() const
{
   return m_interval;
}

std::array<Index1, 2> Index1::refine() const
{
   const Level nxtLevel = getLevel() + 1;
   const Rational del{ 1, 1 << nxtLevel };
   const Rational start = m_interval.getLower();
   const Interval<Rational> intv0(start, start + del);
   const Interval<Rational> intv1(start + del, start + 2 * del);
   return { Index1(intv0), Index1(intv1) };
}

Rational Index1::getMeasure() const
{
   return m_interval.getMeasure();
}

std::string Index1::toString() const
{
   return ToString(getInterval());
}

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