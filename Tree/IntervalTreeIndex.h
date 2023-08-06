#pragma once

#include "Rational.h"
#include "Interval.h"
#include "IntervalTreeDefines.h"
#include "Defines.h"

#include <vector>
#include <array>
#include <unordered_map>

namespace IntervalTree
{
   using FlyWeightKey = int;

   class Index1
   {
   public:
      Index1(const Interval<Rational>& interval);
      Level getLevel() const;
      int getPositionInLevel() const;
      FlyWeightKey getKey() const;
      const Interval<Rational>& getInterval() const;
      std::array<Index1,2> refine() const;
   private:
      Interval<Rational> m_interval;
      Level m_level;
      int m_positionInLevel;
      FlyWeightKey m_key;
   };

   class Index1FlyWeightFactory
   {
   public:
      const Index1* operator()(FlyWeightKey key) const;
      FlyWeightKey add(const Index1& index1);
      FlyWeightKey add(const Interval<Rational>& interval);

   private:
      std::unordered_map<FlyWeightKey, Index1> m_cache;
   };


   template<int N>
   class Index
   {
   public:
      Index(std::array<FlyWeightKey, N> keys, const Index1FlyWeightFactory& factory);
      Level getLevel() const;
      const Interval<Rational>& getInterval(int n) const;
   private:
      const Index1FlyWeightFactory& m_factory1;
      std::array<FlyWeightKey, N> m_keys;
   };

   template<int N>
   class IndexFactory
   {
   public:
      Index<N> get(const std::array<Interval<Rational>, N>& keys);
      Index<N> get(std::initializer_list<Interval<Rational>> keys);
   private:
      Index1FlyWeightFactory m_factory;
   };



   // Implementation

   template<int N>
   Index<N>::Index(std::array<FlyWeightKey, N> keys, const Index1FlyWeightFactory& factory) :
      m_factory1(factory), m_keys(std::move(keys))
   {
   }


   template<int N>
   const Interval<Rational>& Index<N>::getInterval(int n) const
   {
      return m_factory1(m_keys.at(n))->getInterval();
   }

   template<int N>
   Level Index<N>::getLevel() const
   {
      Level result = 0;
      for (auto key : m_keys)
      {
         result = std::max(result, m_factory1(key)->getLevel());
      }
      return result;
   }

   
   template<int N>
   Index<N> IndexFactory<N>::get(const std::array<Interval<Rational>, N>& intervals)
   {
      std::array<FlyWeightKey, N> keys;
      std::transform(intervals.begin(), intervals.end(), keys.begin(), [this](const Interval<Rational>& intv) {return m_factory.add(intv); });
      return Index<N>(keys, m_factory);
   }

   template<int N>
   Index<N> IndexFactory<N>::get(std::initializer_list<Interval<Rational>> intervals)
   {
      if (intervals.size() != N)
      {
         throw MyException("IndexFactory<N>::get(initalizer list) expected  size " + std::to_string(N) + ", actual: "  + std::to_string(intervals.size()));
      }
      std::array<FlyWeightKey, N> keys;
      std::transform(intervals.begin(), intervals.end(), keys.begin(), [this](const Interval<Rational>& intv) {return m_factory.add(intv); });
      return Index<N>(keys, m_factory);
   }
}