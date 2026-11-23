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

constexpr size_t numSimpleStrategies = 2;
constexpr size_t numMediumStrategies = 3;
constexpr size_t numHardStrategies = 2;

std::array<IStrategy *, numSimpleStrategies> getSimple()
{
   static StrategyNakedSingle strategyNakedSingle;
   static StrategyHiddenSingle strategyHiddenSingle;
   static std::array<IStrategy *, numSimpleStrategies> simple{&strategyNakedSingle, &strategyHiddenSingle};
   return simple;
}

std::array<IStrategy *, numSimpleStrategies + numMediumStrategies> getMedium()
{
   static StrategyNakedTuples strategyNakedTuples;
   static StrategyHiddenTuple strategyHiddenTuple;
   static StrategyIntersectionElimination strategyIntersectionElimination;
   static std::array<IStrategy *, numMediumStrategies> mediumOnly{
       &strategyNakedTuples,
       &strategyHiddenTuple,
       &strategyIntersectionElimination,
   };
   static std::array<IStrategy *, numSimpleStrategies + numMediumStrategies> medium =
       concat_arrays<IStrategy *, numSimpleStrategies, numMediumStrategies>(getSimple(), mediumOnly);
   return medium;
}

std::array<IStrategy *, numSimpleStrategies + numMediumStrategies + numHardStrategies> getHard()
{
   static StrategyXWing strategyXWing;
   static StrategySimpleColoring strategySimpleColoring;
   static std::array<IStrategy *, numHardStrategies> hardOnly{&strategyXWing, &strategySimpleColoring};
   static std::array<IStrategy *, numSimpleStrategies + numMediumStrategies + numHardStrategies> hard =
       concat_arrays<IStrategy *, numSimpleStrategies + numMediumStrategies, numHardStrategies>(getMedium(), hardOnly);
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
   static auto s_strategies = getSimple();
   static Strategies strategies(s_strategies);
   return strategies;
}

Strategies Strategies::getMediumStrategies()
{
   static auto s_strategies = getMedium();
   static Strategies strategies(s_strategies);
   return strategies;
}

Strategies Strategies::getAllStrategies()
{
   static auto s_strategies = getHard();
   static Strategies strategies(s_strategies);
   return strategies;
}
