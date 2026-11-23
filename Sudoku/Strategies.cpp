#include "Strategies.h"

#include "StrategyHiddenTuple.h"
#include "StrategyIntersectionElimination.h"
#include "StrategyNakedSingle.h"
#include "StrategyNakedTuples.h"
#include "StrategySimpleColoring.h"
#include "StrategyXWing.h"
using namespace Sudoku;

namespace
{

template <typename T, std::size_t N, std::size_t M>
auto concat_arrays(const std::array<T, N> &a, const std::array<T, M> &b)
{
   // assumes `T` is default-constructible
   std::array<T, N + M> r;
   std::copy(std::begin(a), std::end(a), std::begin(r));
   std::copy(std::begin(b), std::end(b), std::begin(r) + N);
   return r;
}

constexpr size_t numSimpleSweepers = 2;
constexpr size_t numMediumSweepers = 3;
constexpr size_t numHardSweepers = 2;

std::array<IStrategy *, numSimpleSweepers> getSimple()
{
   static StrategyNakedSingle sweepNakedSingle;
   static SweepHiddenSingle sweepHiddenSingle;
   static std::array<IStrategy *, numSimpleSweepers> simple{&sweepNakedSingle, &sweepHiddenSingle};
   return simple;
}

std::array<IStrategy *, numSimpleSweepers + numMediumSweepers> getMedium()
{
   static SweepNakedTuples sweepNakedTuples;
   static SweepHiddenTuple sweepHiddenTuple;
   static SweepIntersectionElimination sweepIntersectionElimination;
   static std::array<IStrategy *, numMediumSweepers> mediumOnly{
       &sweepNakedTuples,
       &sweepHiddenTuple,
       &sweepIntersectionElimination,
   };
   static std::array<IStrategy *, numSimpleSweepers + numMediumSweepers> medium =
       concat_arrays<IStrategy *, numSimpleSweepers, numMediumSweepers>(getSimple(), mediumOnly);
   return medium;
}

std::array<IStrategy *, numSimpleSweepers + numMediumSweepers + numHardSweepers> getHard()
{
   static SweepXWing sweepXWing;
   static SweepSimpleColoring sweepSimpleColoring;
   static std::array<IStrategy *, numHardSweepers> hardOnly{&sweepXWing, &sweepSimpleColoring};
   static std::array<IStrategy *, numSimpleSweepers + numMediumSweepers + numHardSweepers> hard =
       concat_arrays<IStrategy *, numSimpleSweepers + numMediumSweepers, numHardSweepers>(getMedium(), hardOnly);
   return hard;
}
}; // namespace

Strategies::Strategies(std::initializer_list<IStrategy *> strategies) : m_strategies(strategies)
{
}

Strategies::Strategies(std::span<IStrategy *> strategies) : m_strategies(strategies.begin(), strategies.end())
{
}

std::span<IStrategy *const> Strategies::get() const
{
   return m_strategies;
}

Strategies Strategies::getSimpleStrategies()
{
   static auto s_sweepers = getSimple();
   static Strategies strategies(s_sweepers);
   return strategies;
}

Strategies Strategies::getMediumStrategies()
{
   static auto s_sweepers = getMedium();
   static Strategies strategies(s_sweepers);
   return strategies;
}

Strategies Strategies::getAllStrategies()
{
   static auto s_sweepers = getHard();
   static Strategies strategies(s_sweepers);
   return strategies;
}
