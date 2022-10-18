#pragma once

#include <span>
#include <memory>

class IRealFunction;
class IMultiVariableRealValuedFunction;

int GetDimension(const IRealFunction&);
void CheckDerivatives(const IRealFunction& fie, std::span< const double> x, std::span< const double> delx);
double Evaluate(const IRealFunction&, std::span< const double> x);
void Derivative(const IRealFunction&, std::span< const double> x, std::span<double> dfdx);
void CheckIndices(const IRealFunction&, int eqn, int var);
std::shared_ptr< IRealFunction> operator+(const IRealFunction&, const IRealFunction&);
//std::shared_ptr< IMultiVariableRealValuedFunction> operator-(const IMultiVariableRealValuedFunction&, const IMultiVariableRealValuedFunction&);
//std::shared_ptr< IMultiVariableRealValuedFunction> operator/(const IMultiVariableRealValuedFunction&, const IMultiVariableRealValuedFunction&);

