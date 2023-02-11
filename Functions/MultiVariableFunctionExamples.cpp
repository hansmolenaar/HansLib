#include "MultiVariableFunctionExamples.h"
#include "MultiVariablePolynomial.h"
#include "MultiVariableRealValuedFunctionNoDerivatives.h"

std::unique_ptr<IMultiVariableRealValuedFunction> MultiVariableFunctionExamples::GetPolynomial(const std::vector< std::pair<std::vector<int>, double>>& terms)
{
   auto result = std::make_unique< MultiVariablePolynomial>(static_cast<int>(terms.front().first.size()));
   for (const auto& term : terms)
   {
      result->Add(term.second, term.first);
   }
   return result;
}

// Defined on [-3,3] x [-2,2]
std::unique_ptr<IMultiVariableRealValuedFunction> MultiVariableFunctionExamples::SixHumpCamelFunction()
{
   std::vector< std::pair<std::vector<int>, double>> terms;
   terms.emplace_back(std::vector<int>{2, 0}, 4.0);
   terms.emplace_back(std::vector<int>{4, 0}, -2.1);
   terms.emplace_back(std::vector<int>{6, 0}, 1.0 / 3.0);
   terms.emplace_back(std::vector<int>{1, 1}, 1.0);
   terms.emplace_back(std::vector<int>{0, 2}, -4.0);
   terms.emplace_back(std::vector<int>{0, 4}, 4.0);
   return GetPolynomial(terms);
}
