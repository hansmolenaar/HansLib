#include "MultiVariableFunctionExamples.h"
#include "MultiVariablePolynomial.h"
#include "MultiVariableRealValuedFunctionNoDerivatives.h"
#include "SingleVariableFunctionExamples.h"
#include "Utilities/Defines.h"
#include <numeric>


namespace
{
   class  SkewedHatSquaredEval : public IMultiVariableFunctionEvaluate
   {
   public:
      SkewedHatSquaredEval(const std::vector<double>& pos);

      int getDimension() const override;
      double operator()(std::span<const double> x) const;
   private:
      std::vector<std::unique_ptr<ISingleVariableRealValuedFunction>> m_hats;
   };

   SkewedHatSquaredEval::SkewedHatSquaredEval(const std::vector<double>& pos) : m_hats(pos.size())
   {
      str::transform(pos, m_hats.begin(), [](double x) {return SingleVariableFunctionExamples::GetSkewedtHatFunction(x); });
   }

   int SkewedHatSquaredEval::getDimension() const
   {
      return static_cast<int>(m_hats.size());
   }

   double SkewedHatSquaredEval::operator()(std::span<const double> x) const
   {
      if (x.size() != m_hats.size())
      {
         throw MyException("SkewedHatSquaredEval::operator dimension problem");
      }
      double result = 0;
      for (size_t n = 0; n < x.size(); ++n)
      {
         const double eval = m_hats.at(n)->Evaluate(x[n]);
         result += eval * eval;
      }
      return result / x.size();
   }
}

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


std::vector<Interval<double>> MultiVariableFunctionExamples::TridBounds(int dim)
{
   const Interval<double> intv(-dim * dim, dim * dim);
   return { static_cast<size_t>(dim), intv };
}

MultiVariableFunctionExamples::Examplefunction MultiVariableFunctionExamples::SkewedHatSquared(const std::vector<double>& pos)
{
   const int dim = static_cast<int>(pos.size());
   Examplefunction result;
   result.Function = std::make_unique<MultiVariableRealValuedFunctionNoDerivatives>(dim, std::make_unique<SkewedHatSquaredEval>(pos));
   result.Domain = std::vector<Interval<double>>(pos.size(), Interval<double>(0, 1));
   result.Maximum = std::make_unique<ExampleExtremumInfo>();
   result.Maximum->Extremum = 1.0;
   result.Maximum->Positions = std::vector<std::vector<double>>{ std::vector<double>(pos.begin(), pos.end()) };
   return result;
}