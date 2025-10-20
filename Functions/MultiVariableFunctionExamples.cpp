#include "MultiVariableFunctionExamples.h"
#include "BoundsCheck.h"
#include "Defines.h"
#include "MultiVariablePolynomial.h"
#include "MultiVariableRealValuedFunctionNoDerivatives.h"
#include "SingleVariableFunctionExamples.h"
#include <numeric>

namespace
{
class SkewedHatSquaredEval : public IMultiVariableFunctionEvaluate
{
  public:
    SkewedHatSquaredEval(const std::vector<double> &pos);

    int getDimension() const override;
    double operator()(std::span<const double> x) const;

  private:
    std::vector<std::shared_ptr<ISingleVariableRealValuedFunction>> m_hats;
};

SkewedHatSquaredEval::SkewedHatSquaredEval(const std::vector<double> &pos) : m_hats(pos.size())
{
    str::transform(pos, m_hats.begin(),
                   [](double x) { return SingleVariableFunctionExamples::GetSkewedtHatFunction(x); });
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

class ProductEval : public IMultiVariableFunctionEvaluate
{
  public:
    ProductEval(std::vector<std::shared_ptr<ISingleVariableRealValuedFunction>> &functions1D);

    int getDimension() const override;
    double operator()(std::span<const double> x) const;

  private:
    std::vector<std::shared_ptr<ISingleVariableRealValuedFunction>> m_functions;
};

ProductEval::ProductEval(std::vector<std::shared_ptr<ISingleVariableRealValuedFunction>> &functions1D)
{
    for (size_t n = 0; n < functions1D.size(); ++n)
    {
        m_functions.emplace_back(std::move(functions1D.at(n)));
    }
}

int ProductEval::getDimension() const
{
    return static_cast<int>(m_functions.size());
}

double ProductEval::operator()(std::span<const double> x) const
{
    if (x.size() != m_functions.size())
    {
        throw MyException("ProductEval::operator dimension problem");
    }
    double result = 1;
    for (size_t n = 0; n < x.size(); ++n)
    {
        const double eval = m_functions.at(n)->Evaluate(x[n]);
        result *= eval;
    }
    return result;
}

class SumOfSqauresEval : public IMultiVariableFunctionEvaluate
{
  public:
    SumOfSqauresEval(std::vector<std::shared_ptr<ISingleVariableRealValuedFunction>> &functions1D);

    int getDimension() const override;
    double operator()(std::span<const double> x) const;

  private:
    std::vector<std::shared_ptr<ISingleVariableRealValuedFunction>> m_functions;
};

SumOfSqauresEval::SumOfSqauresEval(std::vector<std::shared_ptr<ISingleVariableRealValuedFunction>> &functions1D)
{
    for (size_t n = 0; n < functions1D.size(); ++n)
    {
        m_functions.emplace_back(std::move(functions1D.at(n)));
    }
}

int SumOfSqauresEval::getDimension() const
{
    return static_cast<int>(m_functions.size());
}

double SumOfSqauresEval::operator()(std::span<const double> x) const
{
    if (x.size() != m_functions.size())
    {
        throw MyException("SumOfSqauresEval::operator dimension problem");
    }
    double result = 0;
    for (size_t n = 0; n < x.size(); ++n)
    {
        const double eval = m_functions.at(n)->Evaluate(x[n]);
        result += eval * eval;
    }
    return result;
}
} // namespace

std::shared_ptr<IMultiVariableRealValuedFunction> MultiVariableFunctionExamples::GetPolynomial(
    const std::vector<std::pair<std::vector<int>, double>> &terms)
{
    auto result = std::make_unique<MultiVariablePolynomial>(static_cast<int>(terms.front().first.size()));
    for (const auto &term : terms)
    {
        result->Add(term.second, term.first);
    }
    return result;
}

// Defined on [-3,3] x [-2,2]
std::shared_ptr<IMultiVariableRealValuedFunction> MultiVariableFunctionExamples::SixHumpCamelFunction()
{
    std::vector<std::pair<std::vector<int>, double>> terms;
    terms.emplace_back(std::vector<int>{2, 0}, 4.0);
    terms.emplace_back(std::vector<int>{4, 0}, -2.1);
    terms.emplace_back(std::vector<int>{6, 0}, 1.0 / 3.0);
    terms.emplace_back(std::vector<int>{1, 1}, 1.0);
    terms.emplace_back(std::vector<int>{0, 2}, -4.0);
    terms.emplace_back(std::vector<int>{0, 4}, 4.0);
    return GetPolynomial(terms);
}

MultiVariableFunctionExamples::Examplefunction MultiVariableFunctionExamples::SkewedHatSquared(
    const std::vector<double> &pos)
{
    const int dim = static_cast<int>(pos.size());
    Examplefunction result;
    auto function = std::make_shared<MultiVariableRealValuedFunctionNoDerivatives>(
        dim, std::make_shared<SkewedHatSquaredEval>(pos));
    result.Function = std::make_shared<MultiVariableRealValuedFunctionEvaluateCached>(function);
    result.Domain = std::vector<Interval<double>>(pos.size(), Interval<double>(0, 1));
    result.Maximum = std::make_unique<ExampleExtremumInfo>();
    result.Maximum->Extremum = 1.0;
    result.Maximum->Positions = std::vector<std::vector<double>>{std::vector<double>(pos.begin(), pos.end())};
    return result;
}

std::shared_ptr<IMultiVariableFunctionEvaluate> MultiVariableFunctionExamples::SumOfSquares(
    std::vector<std::shared_ptr<ISingleVariableRealValuedFunction>> &functions)
{
    std::shared_ptr<IMultiVariableFunctionEvaluate> result = std::make_shared<SumOfSqauresEval>(functions);
    return result;
}

MultiVariableFunctionExamples::Examplefunction MultiVariableFunctionExamples::DiscontinuousHatProduct(
    const std::vector<double> &xpeak, const std::vector<double> &ypeakLeft, const std::vector<double> &ypeakRight)
{
    Utilities::MyAssert(xpeak.size() == ypeakLeft.size());
    Utilities::MyAssert(xpeak.size() == ypeakRight.size());
    BoundsCheck<double>::Create(0.0, 1.0).check(*str::max_element(xpeak));
    BoundsCheck<double>::CreateIsPositive().check(*str::max_element(ypeakLeft));
    BoundsCheck<double>::CreateIsPositive().check(*str::max_element(ypeakRight));

    const int dim = static_cast<int>(xpeak.size());
    std::vector<std::shared_ptr<ISingleVariableRealValuedFunction>> functions;
    for (size_t n = 0; n < xpeak.size(); ++n)
    {
        functions.emplace_back(SingleVariableFunctionExamples::GetDiscontinuousHatFunction(xpeak.at(n), ypeakLeft.at(n),
                                                                                           ypeakRight.at(n)));
    }
    Examplefunction result;
    auto function =
        std::make_shared<MultiVariableRealValuedFunctionNoDerivatives>(dim, std::make_shared<ProductEval>(functions));
    result.Function = std::make_shared<MultiVariableRealValuedFunctionEvaluateCached>(function);
    result.Domain = std::vector<Interval<double>>(xpeak.size(), Interval<double>(0, 1));
    result.Maximum = std::make_unique<ExampleExtremumInfo>();
    result.Maximum->Extremum = 1.0;
    for (size_t n = 0; n < xpeak.size(); ++n)
    {
        result.Maximum->Extremum *= std::max(ypeakLeft.at(n), ypeakRight.at(n));
    }
    result.Maximum->Positions = std::vector<std::vector<double>>{std::vector<double>(xpeak.begin(), xpeak.end())};

    return result;
}

MultiVariableFunctionExamples::Examplefunction MultiVariableFunctionExamples::SumOfSquares(int dim)
{
    Examplefunction result;

    std::vector<std::pair<std::vector<int>, double>> powsAndCoefs;
    for (int d = 0; d < dim; ++d)
    {
        std::vector<int> pows(dim, 0);
        pows[d] = 2;
        powsAndCoefs.emplace_back(pows, 1.0);
    }
    auto function = GetPolynomial(powsAndCoefs);
    result.Function = std::make_shared<MultiVariableRealValuedFunctionEvaluateCached>(std::move(function));

    result.Domain = std::vector<Interval<double>>(dim, Interval<double>(-1, 1));
    result.Minimum = std::make_unique<ExampleExtremumInfo>();
    result.Minimum->Extremum = 0.0;
    result.Minimum->Positions.emplace_back(std::vector<double>(dim, 0.0));

    return result;
}