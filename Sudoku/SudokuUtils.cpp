#include "SudokuUtils.h"
#include "CandidatesGrid.h"
#include "House.h"

using namespace Sudoku;

HouseByMultiplicities::HouseByMultiplicities(const CandidatesGrid &cg, const House &house)
{
   for (auto cell : house.cells)
   {
      const Field field = cell.field;
      const auto numCandidates = cg[field].size();
      m_multiplicities.at(numCandidates).push_back(field);
   }
}

std::span<const Field> HouseByMultiplicities::get(Index idx) const
{
   return m_multiplicities.at(idx);
}
