#pragma once

#include "CandidatesGrid.h"
#include "FieldValue.h"

namespace TestUtils
{
void CheckFieldValues(const Sudoku::CandidatesGrid &, const Sudoku::FieldValue::Container &);

} // namespace TestUtils
