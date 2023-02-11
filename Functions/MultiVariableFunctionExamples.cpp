#include "MultiVariableFunctionExamples.h"
#include "MultiVariablePolynomial.h"
#include "MultiVariableRealValuedFunctionNoDerivatives.h"

#if false
namespace
{

   class IScaleInput
   {
   public:
      virtual ~IScaleInput() noexcept = default;
      virtual std::vector<double> Scale(std::span<const double>) const = 0;
   };

   class ScaledFunction : public MultiVariableRealValuedFunctionNoDerivatives
   {
   public:
      ScaledFunction(std::unique_ptr<IMultiVariableRealValuedFunction>&& fie,
         std::unique_ptr<IScaleInput>&& scaleXyz);

      double operator()(std::span<const double> xy) const override;

   private:
      std::unique_ptr<IScaleInput> m_xyzScaling;
      std::unique_ptr<IMultiVariableRealValuedFunction> m_function;
   };

   ScaledFunction::ScaledFunction(std::unique_ptr<IMultiVariableRealValuedFunction>&& fie,
      std::unique_ptr<IScaleInput>&& scaleXyz) :
      m_xyzScaling(std::move(scaleXyz)), m_function(std::move(fie))
   {}

   double  ScaledFunction::operator()(std::span<const double> xy) const
   {
      const auto scaledInput = m_xyzScaling->Scale(xy);
      return (*m_function)(scaledInput);
   }

   class ScaledFunctionNoDerivativesGiven : public FunctionNoDerivativesGiven
   {
   public:
      ScaledFunctionNoDerivativesGiven(int dim, std::unique_ptr<IMultiVariableRealValuedFunction>&& fie,
         std::unique_ptr<IScaleInput>&& scaleXyz) :
         FunctionNoDerivativesGiven(dim, std::make_unique<ScaledFunction>(std::move(fie), std::move(scaleXyz)))
      {}
   };


}
#endif

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

#if false
std::unique_ptr<IMultiVariableRealValuedFunction> MultiVariableFunctionExamples::ScaleInput(
   std::unique_ptr<IMultiVariableRealValuedFunction>&& fie,
   std::unique_ptr<std::function<std::vector<double>(const std::vector<double>&)>>&& scaleXyz)
{
   throw MyException("MultiVariableFunctionExamples::ScaleInput not implemented");

   auto fie = MultiVariableFunctionExamples::SixHumpCamelFunction();
   auto scale = [](std::span<const double> s) {return std::vector<double>(s.begin(), s.end()); };
   auto scalePtr = new  [](std::span<const double> s) {return std::vector<double>(s.begin(), s.end()); };
   std::unique_ptr<IMultiVariableRealValuedFunction> fie =
      std::unique_ptr<std::function<std::vector<double>(std::span<const double>)>> && scaleXyz);

      std::function<std::vector<double>(std::span<const double>)> fie = ScaledFunction(std::move(fie), std::move(scaleXyz));

      FunctionNoDerivativesGiven(dim, std::make_unique<std::function<std::vector<double>(std::span<const double>)>>(ScaledFunction(std::move(fie), std::move(scaleXyz))))

         return std::make_unique<ScaledFunction>(fie, scaleXyz);

}
#endif