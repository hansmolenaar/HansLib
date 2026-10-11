#include "CandidatesGrid.h"
#include "Cell.h"
#include "MyAssert.h"
#include "SweepNakedSingle.h"

#include <numeric>

using namespace Sudoku;
using namespace Utilities;

Index CandidatesGrid::getNumCandidates() const
{
   return std::accumulate(m_candidates.begin(), m_candidates.end(), static_cast<Index>(0),
                          [](Index acc, const Candidates &c) { return acc + c.size(); });
}

void CandidatesGrid::setCandidates(Field field, std::initializer_list<Value> values)
{
   m_candidates.at(field).setOnly(values);
}

void CandidatesGrid::removeCandidate(Field field, Value value)
{
   m_candidates.at(field).unsetValue(value);
}

void CandidatesGrid::setGiven(Field field, Value value)
{
   MyAssert((*this)[field].size() == NumValue);
   setCandidates(field, {value});
}

void CandidatesGrid::setGiven(Index row, Index col, Value value)
{
   const Field field = Cell::getFromRowCol(row, col).field;
   setGiven(field, value);
}

void CandidatesGrid::update()
{
   SweepNakedSingle sweeper;
   ISweep::iterate(sweeper, *this);
}
