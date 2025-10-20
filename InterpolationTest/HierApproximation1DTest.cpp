#include <gtest/gtest.h>

#include "HierApproximation.h"
#include "HierBasisFunction.h"
#include "HierBasisFunction1D_ExtendedLevelOneBC.h"
#include "HierBasisFunction1D_ExtraplolateBC.h"
#include "HierBasisFunction1D_HomogenousBC.h"
#include "ISingleVariableRealValuedFunctionUtils.h"
#include "MyException.h"
#include "NodeRefinePredicateFactoryByLevel.h"
#include "NodeRefinePredicateFactoryByLevelOrSurplus.h"
#include "Plotting.h"
#include "Single.h"
#include "SingleVariableFunctionExamples.h"
#include "SingleVariablePolynomial.h"
#include "SingleVariableRealValuedFunction.h"

#include <filesystem>
#include <fstream>

inline constexpr double Epsilon = 1.0e-10;

namespace
{
void TestCollocationPoints(const ISingleVariableRealValuedFunction &expect, const HierApproximation &approximation)
{
    for (const auto &point : approximation.getCollocationPoints())
    {
        const double x = Utilities::Single(point);
        ASSERT_NEAR(approximation(std::vector<double>{x}), expect.Evaluate(x), Epsilon);
    }
}

void CollocationPointsToFile(std::string functionName, const HierApproximation &approx)
{
    std::ofstream ofs(Plotting::GetFile(functionName));
    ofs << "x" << " , " << "y" << "\n";
    for (const auto &xvec : approx.getCollocationPoints())
    {
        const double x = xvec.front();
        const double eval = approx(std::vector<double>{x});
        ofs << x << " , " << eval << "\n";
    }
    ofs.close();
}
}; // namespace

TEST(HierApproximation1DTest, Basis_1_1)
{
    HierBasisFunction1D_HomogenousBC_Factory factory1D;
    HierBasisFunction_Factory factory(size_t{1}, &factory1D);
    NodeRefinePredicateFactoryByLevel predicate(0);
    const HierBasisFunction1D_HomogenousBC functionToApproximate(HierLevelIndex(1, 1));
    const auto approximation = HierApproximation::Create(functionToApproximate, factory, predicate);
    ASSERT_EQ(approximation->getCollocationPoints().size(), 1);
    ASSERT_NEAR((*approximation)(std::vector<double>{0.25}), 0.5, Epsilon);
    ASSERT_NEAR((*approximation)(std::vector<double>{0.5}), 1.0, Epsilon);
    ASSERT_NEAR((*approximation)(std::vector<double>{0.75}), 0.5, Epsilon);
}

TEST(HierApproximation1DTest, CubicPolynomialHomogeneousBC_level2)
{
    HierBasisFunction1D_HomogenousBC_Factory factory1D;
    HierBasisFunction_Factory factory(size_t{1}, &factory1D);
    NodeRefinePredicateFactoryByLevel predicate(1);
    auto functionToApproximate = SingleVariableRealValuedFunction([](double x) { return x * x * (1 - x); });
    const auto approximation = HierApproximation::Create(functionToApproximate, factory, predicate);
    ASSERT_EQ(approximation->getCollocationPoints().size(), 3);
    TestCollocationPoints(functionToApproximate, *approximation);

    // ISingleVariableRealValuedFunctionUtils::ToFile(functionToApproximate, *approximation, 0.0, 1.0, 1000,
    // "CubicPolynomialHomogeneousBC_level2");
}

TEST(HierApproximation1DTest, CubicPolynomialHomogeneousBC_level5)
{
    HierBasisFunction1D_HomogenousBC_Factory factory1D;
    HierBasisFunction_Factory factory(size_t{1}, &factory1D);
    NodeRefinePredicateFactoryByLevel predicate(4);
    auto functionToApproximate = SingleVariableRealValuedFunction([](double x) { return x * x * (1 - x); });
    const auto approximation = HierApproximation::Create(functionToApproximate, factory, predicate);
    ASSERT_EQ(approximation->getCollocationPoints().size(), 31);
    TestCollocationPoints(functionToApproximate, *approximation);

    // ISingleVariableRealValuedFunctionUtils::ToFile(functionToApproximate, *approximation, 0.0, 1.0, 1000,
    // "CubicPolynomialHomogeneousBC_level5");
}

TEST(HierApproximation1DTest, Basis_Extended)
{
    SingleVariablePolynomial functionToApproximate;
    functionToApproximate.Add(2.0, SingleVariableMonomial(0));
    functionToApproximate.Add(3.0, SingleVariableMonomial(1));

    HierBasisFunction1D_ExtendedLevelOneBC_Factory factory1D;
    HierBasisFunction_Factory factory(size_t{1}, &factory1D);
    NodeRefinePredicateFactoryByLevel predicate(0);

    const auto approximation = HierApproximation::Create(functionToApproximate, factory, predicate);
    ASSERT_EQ(approximation->getCollocationPoints().size(), 3);
    TestCollocationPoints(functionToApproximate, *approximation);
}

TEST(HierApproximation1DTest, Square)
{
    const SingleVariableMonomial square(2);
    constexpr size_t maxLevel = 8;
    std::vector<double> maxSurplus;
    for (int n = 1; n < maxLevel; ++n)
    {
        HierBasisFunction1D_ExtendedLevelOneBC_Factory factory1D;
        HierBasisFunction_Factory factory(size_t{1}, &factory1D);
        NodeRefinePredicateFactoryByLevel predicate(n);

        const auto approximation = HierApproximation::Create(square, factory, predicate);
        TestCollocationPoints(square, *approximation);
        maxSurplus.push_back(approximation->getMaxSurplus());
    }

    for (size_t n = 1; n < maxSurplus.size(); ++n)
    {
        ASSERT_TRUE(maxSurplus.at(n) < 0.5 * maxSurplus.at(n - 1));
    }
}

TEST(HierApproximation1DTest, Basis_Extrapolate)
{
    SingleVariablePolynomial functionToApproximate;
    functionToApproximate.Add(2.0, SingleVariableMonomial(0));
    functionToApproximate.Add(3.0, SingleVariableMonomial(1));

    HierBasisFunction1D_ExtraplolateBC_Factory factory1D;
    HierBasisFunction_Factory factory(size_t{1}, &factory1D);
    NodeRefinePredicateFactoryByLevel predicate(0);

    const auto approximation = HierApproximation::Create(functionToApproximate, factory, predicate);
    ASSERT_EQ(approximation->getCollocationPoints().size(), 1);
    TestCollocationPoints(functionToApproximate, *approximation);
}

TEST(HierApproximation1DTest, Square_Extrapolate)
{
    const SingleVariableMonomial functionToApproximate(2);

    HierBasisFunction1D_ExtraplolateBC_Factory factory1D;
    HierBasisFunction_Factory factory(size_t{1}, &factory1D);
    NodeRefinePredicateFactoryByLevel predicate(3);

    const auto approximation = HierApproximation::Create(functionToApproximate, factory, predicate);
    TestCollocationPoints(functionToApproximate, *approximation);
    // ISingleVariableRealValuedFunctionUtils::ToFile(functionToApproximate, *approximation, 0.0, 1.0, 1000,
    // "Square_Extrapolate");
}

TEST(HierApproximation1DTest, Square_ExtrapolateSurplus)
{
    const SingleVariableMonomial square(2);
    constexpr size_t maxLevel = 8;
    std::vector<double> maxSurplus;
    for (int n = 1; n < maxLevel; ++n)
    {
        HierBasisFunction1D_ExtraplolateBC_Factory factory1D;
        HierBasisFunction_Factory factory(size_t{1}, &factory1D);
        NodeRefinePredicateFactoryByLevel predicate(n);

        const auto approximation = HierApproximation::Create(square, factory, predicate);
        TestCollocationPoints(square, *approximation);
        maxSurplus.push_back(approximation->getMaxSurplus());
    }

    for (size_t n = 2; n < maxSurplus.size(); ++n)
    {
        ASSERT_TRUE(maxSurplus.at(n) < 0.5 * maxSurplus.at(n - 1));
    }
}

TEST(HierApproximation1DTest, Runge_Extrapolate_Adaptive)
{
    const auto fiePtr = SingleVariableFunctionExamples::GetRungeFunction();
    auto runge = [&fiePtr](const double x) { return fiePtr->Evaluate(2 * x - 1); };
    const SingleVariableRealValuedFunction functionToApproximate(runge);

    HierBasisFunction1D_ExtraplolateBC_Factory factory1D;
    HierBasisFunction_Factory factory(size_t{1}, &factory1D);
    NodeRefinePredicateFactoryByLevelOrSurplus predicate(1, 1.0e-3);
    // auto predicate = [](const HierRefinementInfo& hri) {return hri.MultiLevelIndex.get().front().getLevel() < 2 ||
    // (hri.Surplus > 1.0e-3); };

    const auto approximation = HierApproximation::Create(functionToApproximate, factory, predicate);
    const auto points = approximation->getCollocationPoints();
    ASSERT_EQ(points.size(), 135);
    TestCollocationPoints(functionToApproximate, *approximation);
    // CollocationPointsToFile("Runge_Extrapolate_Adaptive_Points", *approximation);
    // ISingleVariableRealValuedFunctionUtils::ToFile(functionToApproximate, *approximation, 0.0, 1.0, 1000,
    // "Runge_Extrapolate_Adaptive");
}

TEST(HierApproximation1DTest, Hat_Adaptive)
{
    const double xpeak = std::sqrt(0.5);
    const auto fiePtr = SingleVariableFunctionExamples::GetSkewedtHatFunction(xpeak);
    auto hat = [&fiePtr](const double x) { return fiePtr->Evaluate(x); };
    const SingleVariableRealValuedFunction functionToApproximate(hat);

    HierBasisFunction1D_HomogenousBC_Factory factory1D;
    HierBasisFunction_Factory factory(size_t{1}, &factory1D);
    NodeRefinePredicateFactoryByLevelOrSurplus predicate(3, 1.0e-5);
    // auto predicate = [](const HierRefinementInfo& hri) {return hri.MultiLevelIndex.get().front().getLevel() < 4 ||
    // (hri.Surplus > 1.0e-5); };

    const auto approximation = HierApproximation::Create(functionToApproximate, factory, predicate);
    const auto points = approximation->getCollocationPoints();
    ASSERT_EQ(points.size(), 41);
    TestCollocationPoints(functionToApproximate, *approximation);
    CollocationPointsToFile("HierApproximation1DTest_Hat_Adaptive_Points", *approximation);
    // ISingleVariableRealValuedFunctionUtils::ToFile(functionToApproximate, *approximation, 0.0, 1.0, 1000,
    // "HierApproximation1DTest_Hat_Adaptive");
}
