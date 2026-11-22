#pragma once

#include "CandidatesGrid.h"
#include "FieldValue.h"
#include "FieldsInfo.h"
#include "GridChange.h"
#include "IStrategy.h"

namespace TestUtils
{
void CheckFieldValues(const Sudoku::CandidatesGrid &, const Sudoku::FieldValue::Container &);
void CheckFieldInfo(const Sudoku::FieldsInfo &);
void CheckStrategyInterface(Sudoku::IStrategy &);
void CheckStrategy(Sudoku::IStrategy &, const Sudoku::CandidatesGrid &, const Sudoku::GridChange &);
void ShowStrategy(Sudoku::IStrategy &, const Sudoku::CandidatesGrid &);

} // namespace TestUtils
