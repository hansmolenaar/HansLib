#include "Functions/SingleVariableFunctionExamples.h"
#include "Functions/SingleVariableRealValuedFunction.h"



std::unique_ptr<ISingleVariableRealValuedFunction> SingleVariableFunctionExamples::GetRungeFunction()
{
   return std::make_unique<SingleVariableRealValuedFunction>([](double x) {return 1.0 / (1.0 + 25.0 * x * x); });
}
