#pragma once
#include <string>

class ISingleVariableRealValuedFunction;

namespace ISingleVariableRealValuedFunctionUtils
{
   double Evaluate(ISingleVariableRealValuedFunction& fie, double x);
   void CheckDerivative(ISingleVariableRealValuedFunction& fie, double x, double delx, bool isLinear = false);
   void ToFile(const ISingleVariableRealValuedFunction& fie, double xmin, double xmax, int nPoints, const std::string& functionName, std::string folderName = "");
   void ToFile(const ISingleVariableRealValuedFunction& expect, const ISingleVariableRealValuedFunction& approximate, double xmin, double xmax, int nPoints, const std::string& functionName, std::string folderName = "");
}
