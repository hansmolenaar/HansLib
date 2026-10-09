#include "Grid.h"

#include <numeric>

using namespace Sudoku;

void Grid::setGiven(Field field, Value value)
{
   m_grid.at(field).setGiven(value);
}

Index Grid::getNumCandidates() const
{
   return std::accumulate(m_grid.begin(), m_grid.end(), static_cast<Index>(0),
                          [](Index acc, const Candidates &c) { return acc + c.size(); });
}
