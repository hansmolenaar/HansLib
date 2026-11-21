#pragma once

#include "CandidatesGrid.h"
#include "FieldValue.h"
#include "FieldsInfo.h"

namespace TestUtils
{
void CheckFieldValues(const Sudoku::CandidatesGrid &, const Sudoku::FieldValue::Container &);
void CheckFieldInfo(const Sudoku::FieldsInfo &);

} // namespace TestUtils
