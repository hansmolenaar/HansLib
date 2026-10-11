#include "CandidatesGrid.h"

#include <numeric>

using namespace Sudoku;

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
