#pragma once
#include <string>

class ISingleVariableRealValuedFunction;

namespace ISingleVariableRealValuedFunctionUtils
{
   double Evaluate(ISingleVariableRealValuedFunction& fie, double x);
   void ToFile(const ISingleVariableRealValuedFunction& fie, double xmin, double xmax, int nPoints, const std::string& functionName);
   void ToFile(const ISingleVariableRealValuedFunction& expect, const ISingleVariableRealValuedFunction& approximate, double xmin, double xmax, int nPoints, const std::string& functionName);
}
