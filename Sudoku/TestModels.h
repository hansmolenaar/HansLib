#pragma once

#include "Grid.h"

namespace Sudoku
{
namespace TestModels
{
CandidatesGrid getSolved();
// TODO  Grid fromString(const std::string &);
}; // namespace TestModels

#if false

Diagram getOneStar1();
Diagram getOneStar2();
Diagram getTwoStar1();
Diagram getThreeStar1();
Diagram getThreeStar2();
Diagram getFourStar1();
Diagram getFourStar2();
Diagram getFourStar3();

Diagram getBiValue1();

#endif
} // namespace Sudoku
