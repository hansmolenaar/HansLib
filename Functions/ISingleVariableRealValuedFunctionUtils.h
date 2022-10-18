#pragma once

class ISingleVariableRealValuedFunction;

double Evaluate(ISingleVariableRealValuedFunction& fie, double x);
void CheckDerivative(ISingleVariableRealValuedFunction& fie, double x, double delx, bool isLinear = false);
