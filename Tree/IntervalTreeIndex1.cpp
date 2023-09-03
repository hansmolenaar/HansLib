#include "IntervalTreeIndex1.h"

#include "MyException.h"
#include "MyAssert.h"
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
   m_key(composeKey(m_level, m_positionInLevel))
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

std::tuple<Level, int> Index1::decomposeKey(int key)
{
   int maxKey = 0;
   for (Level level = 0; level < 256 ; ++level)
   {
      if (key < maxKey + (1 << level)) return std::make_tuple(level, key - maxKey);
      maxKey += 1 << level;
   }
   throw MyException("Index1::decomposeKey should not come here");
}

Index1::Key Index1::composeKey(Level level, int positionInLevel)
{
   const Index1::Key pow2m1 = (1 << level) - 1;
   return pow2m1 + positionInLevel;
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

bool Index1::isRoot() const
{
   return m_level == 0;
}

Index1 Index1::CreateRoot()
{
   return Index1(Interval<Rational>{ {0, 1}, { 1,1 }});
}

Rational Index1::getCenter() const
{
   return m_interval.interpolate(Rational(1, 2));
}

Index1 Index1::getSibling() const
{
   Utilities::MyAssert(!isRoot());
   if (m_positionInLevel % 2 == 0)
   {
      auto [succes, result] = getAdjacentInDir(true);
      Utilities::MyAssert(succes);
      return result;
   }
   else
   {
      auto [succes, result] = getAdjacentInDir(false);
      Utilities::MyAssert(succes);
      return result;
   }
}

std::tuple<bool, Index1> Index1::getSiblingInDir(bool posDir) const
{
   const bool posKid = (m_positionInLevel % 2 != 0);
   if (m_level == 0 || posKid == posDir) return { false, CreateRoot() };
   return { true, getSibling() };
}

std::tuple<bool, Index1> Index1::getAdjacentInDir(bool posDir) const
{
   if (m_level == 0) return { false, CreateRoot() };
   if (posDir)
   {
      if (m_positionInLevel + 1 >= (1 << m_level)) return { false, CreateRoot() };
      const Rational upr = m_interval.getUpper();
      return { true, Index1(Interval<Rational>(upr, upr + getMeasure())) };
   }
   else
   {
      if (m_positionInLevel == 0) return { false, CreateRoot() };
      const Rational lwr = m_interval.getLower();
      return { true, Index1(Interval<Rational>(lwr - getMeasure(), lwr)) };
   }
}

Index1 Index1::getParent() const
{
   Utilities::MyAssert(!isRoot());
   auto intv = getInterval();
   const auto siblingIntv = getSibling().getInterval();
   intv.add(siblingIntv.getLower());
   intv.add(siblingIntv.getUpper());
   return Index1(intv);
}