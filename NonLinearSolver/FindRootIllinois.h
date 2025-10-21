#pragma once

#include "IFindRoot.h"

class FindRootIllinois : public IFindRoot
{
  public:
    NonLinearSolverStatus FindInInterval(ISingleVariableRealValuedFunction &fie, double xmin, double xmax,
                                         INonLinearSolverStrategy &strategy, double &result) override;
};
