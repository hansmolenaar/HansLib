#include <gtest/gtest.h>

#include "Interpolation/HierApproximation.h"
#include "Interpolation/HierBasisFunction1D_HomogenousBC.h"
#include "Utilities/MyException.h"
#include "Functions/IMultiVariableRealValuedFunction.h"
#include "Utilities/Single.h"

#include "Functions/SingleVariableFunctionExamples.h"
#include "Utilities/Plotting.h"

#include <filesystem>
#include <fstream>

inline constexpr double Epsilon = 1.0e-10;
