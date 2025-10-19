#include <gtest/gtest.h>

#include "RealFunctionCheckDerivative.h"

#include "SingleVariableMonomial.h"
#include "MultiVariableMonomial.h"

const double c_eps = 1.0e-10;

TEST(RealFunctionCheckDerivativeTest, CheckSingleSucces)
{
   SingleVariableMonomial fie(2);
   RealFunctionCheckDerivative::Check(fie, 2.0, 1.0e-3 );
}

class WrongDerivative : public ISingleVariableRealValuedFunction
{
public:
	double Evaluate(double x)const override { return x*x; }
	double Derivative(double x)const override { return x;}
	bool IsNonConstant() const override { return true; }
	bool HasDerivative() const override { return true; }
};


TEST(RealFunctionCheckDerivativeTest, CheckSingleFailure)
{
   WrongDerivative fie;
    // this tests _that_ the expected exception is thrown
    EXPECT_THROW({
        try
        {
           RealFunctionCheckDerivative::Check(fie, 2.0, 1.0e-3 );
        }
        catch( const MyException& e )
        {
            // and this tests that it has the correct message
            EXPECT_STREQ( "Problem with derivative!", e.what() );
            throw;
        }
    }, MyException );
}


TEST(RealFunctionCheckDerivativeTest, CheckMultipleSucces)
{
   auto fie = MultiVariableMonomial({3,2});
   RealFunctionCheckDerivative::Check(fie, std::vector<double>{4,2}, std::vector<double>{1.0e-3, 1.0e-3} );
}

class WrongDerivatives : public IMultiVariableRealValuedFunction
{
public:
	int GetDomainDimension() const override { return 2; }
	double Evaluate(std::span<const double>x)const override { return x[0]*x[1]; }
	void Derivative(std::span<const double>x, std::span< double> dfdx)const override {dfdx[0] = 2; dfdx[1] = 2; }
	bool DerivativeAlwaysZero(int var) const override { return false; }
	bool HasDerivative() const override { return true; }
};



TEST(RealFunctionCheckDerivativeTest, CheckMultipleFailure)
{
   WrongDerivatives fie;
    // this tests _that_ the expected exception is thrown
    EXPECT_THROW({
        try
        {
           RealFunctionCheckDerivative::Check(fie, std::vector<double>{8, 9}, std::vector<double>{1.0e-2, 1.0e-2} );
        }
        catch( const MyException& e )
        {
            // and this tests that it has the correct message
            EXPECT_STREQ( "Problem with derivative!", e.what() );
            throw;
        }
    }, MyException );
}

