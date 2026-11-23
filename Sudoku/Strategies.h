#pragma once

#include <initializer_list>
#include <span>
#include <vector>

namespace Sudoku
{
class IStrategy;

class Strategies
{
 public:
   explicit Strategies(std::initializer_list<IStrategy *>);
   explicit Strategies(std::span<IStrategy *>);

   std::span<IStrategy *const> get() const;

   static Strategies getSimpleStrategies();
   static Strategies getMediumStrategies();
   static Strategies getAllStrategies();

 private:
   std::vector<IStrategy *> m_strategies;
};

} // namespace Sudoku
