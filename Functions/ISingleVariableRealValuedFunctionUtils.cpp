#include "ISingleVariableRealValuedFunctionUtils.h"
#include "ISingleVariableRealValuedFunction.h"
//#include "NonLinearSolverStrategyResidualReduction.h"
//#include "NonLinearSolver.h"
#include "MyAssert.h"
#include "MyException.h"
#include "Plotting.h"

#include <string>
#include <sstream>
#include <array>
#include <filesystem>
#include <fstream>

double ISingleVariableRealValuedFunctionUtils::Evaluate(ISingleVariableRealValuedFunction& fie, double x)
{
   double result;
   fie.Evaluate(std::span<const double>(&x, 1), std::span<double>(&result, 1));
   return result;
}


void ISingleVariableRealValuedFunctionUtils::ToFile(const ISingleVariableRealValuedFunction& fie, double xmin, double xmax, int nPoints, const std::string& functionName)
{
   Utilities::MyAssert(nPoints > 1);
   Utilities::MyAssert(xmax > xmin);

   std::ofstream ofs(Plotting::GetFile(functionName));
   auto function = [&fie](double x) {return fie.Evaluate(x); };
   const auto values = Plotting::EvaluateFunctions({function}, xmin, xmax, nPoints);
   Plotting::ToStream(ofs, values);
   ofs.close();
}


void ISingleVariableRealValuedFunctionUtils::ToFile(const ISingleVariableRealValuedFunction& expect, const ISingleVariableRealValuedFunction& approximate, double xmin, double xmax, int nPoints, const std::string& functionName)
{
   Utilities::MyAssert(nPoints > 1);
   Utilities::MyAssert(xmax > xmin);

   std::ofstream ofs(Plotting::GetFile(functionName));
   const auto eval = [&expect](double x) {return expect.Evaluate(x); };
   const auto actl = [&approximate](double x) {return approximate.Evaluate(x); };
   const auto values = Plotting::EvaluateFunctions({eval, actl}, xmin, xmax, nPoints );
   Plotting::ToStream(ofs, std::vector<std::string>{"x", "Expect", "Actual"}, values);
   ofs.close();
}
