#pragma once

#include "SudokuDefines.h"

#include <vector>

namespace Sudoku
{
class CandidatesGrid;
struct House;

class HouseByMultiplicities
{
 public:
   HouseByMultiplicities(const CandidatesGrid &, const House &);
   std::span<const Field> get(Index) const;

 private:
   std::array<std::vector<Field>, NumIndex> m_multiplicities;
};
} // namespace Sudoku
