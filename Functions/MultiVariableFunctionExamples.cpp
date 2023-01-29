#include "MultiVariableFunctionExamples.h"
#include "MultiVariablePolynomial.h"



std::unique_ptr<IMultiVariableRealValuedFunction> MultiVariableFunctionExamples::GetPolynomial(const std::vector< std::pair<std::vector<int>, double>>& terms)
{
   auto result = std::make_unique< MultiVariablePolynomial>(static_cast<int>(terms.front().first.size()));
   for (const auto& term : terms)
   {
      result->Add(term.second, term.first);
   }
   return result;
}