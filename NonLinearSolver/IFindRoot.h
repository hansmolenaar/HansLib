#pragma once

#include "NonLinearSolverStatus.h"

class ISingleVariableRealValuedFunction;
class INonLinearSolverStrategy;

class IFindRoot
{
  public:
    // return succes
    virtual NonLinearSolverStatus FindInInterval(ISingleVariableRealValuedFunction &fie, double xmin, double xmax,
                                                 INonLinearSolverStrategy &strategy, double &result) = 0;
    virtual ~IFindRoot() = default;
};
