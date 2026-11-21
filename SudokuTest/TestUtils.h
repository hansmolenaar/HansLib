#pragma once

#include "CandidatesGrid.h"
#include "FieldValue.h"
#include "FieldsInfo.h"
#include "IStrategy.h"

namespace TestUtils
{
void CheckFieldValues(const Sudoku::CandidatesGrid &, const Sudoku::FieldValue::Container &);
void CheckFieldInfo(const Sudoku::FieldsInfo &);
void CheckStrategy(Sudoku::IStrategy &);

} // namespace TestUtils
