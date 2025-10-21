#pragma once

#include "MultiVariableRealValuedFunction.h"
#include <array>
#include <span>

class MatrixKelvinRepr3;
class IRealFunction;

class EigenValueSolverSym3x3 : public IRealFunction
{
  public:
    int GetRangeDimension() const override;
    int GetDomainDimension() const override;
    bool DerivativeAlwaysZero(int eqn, int var) const override;
    void Evaluate(std::span<const double> x, std::span<double> y) const override;
    // TODO derivatives are buggy!
    // TODO Fix tests
    void Derivative(std::span<const double> x, IMatrix &dfdx) const override;
    bool HasDerivative() const override;

    static void CalculateEigenvalues3x3(const MatrixKelvinRepr3 &, std::span<double>);
};

namespace EigenValueSolverSym3x3Utils
{
double EvaluateQ(std::span<const double> x);
double ClipDetB(double detB);

class AuxilaryEquationRoots : public IRealFunction
{
  public:
    int GetRangeDimension() const override;
    int GetDomainDimension() const override;
    bool DerivativeAlwaysZero(int eqn, int var) const override;
    bool HasDerivative() const override;

    // Roots of:  x^3 -3x - detB = 0
    void Evaluate(std::span<const double> x, std::span<double> y) const override;

    // Derivatives wrt detB
    void Derivative(std::span<const double> x, IMatrix &dfdx) const override;
};

class FunctionP : public IMultiVariableRealValuedFunction
{
  public:
    FunctionP();
    int GetDomainDimension() const override;
    double Evaluate(std::span<const double> x) const override;
    void Derivative(std::span<const double> x, std::span<double> dfdx) const override;
    bool DerivativeAlwaysZero(int var) const override;
    bool HasDerivative() const override;

    using IMultiVariableRealValuedFunction::Derivative;
    using IMultiVariableRealValuedFunction::DerivativeAlwaysZero;
};

class FunctionP3Inv : public IMultiVariableRealValuedFunction
{
  public:
    int GetDomainDimension() const override;
    double Evaluate(std::span<const double> x) const override;
    void Derivative(std::span<const double> x, std::span<double> dfdx) const override;
    bool DerivativeAlwaysZero(int var) const override;
    bool HasDerivative() const override;

    using IMultiVariableRealValuedFunction::Derivative;
    using IMultiVariableRealValuedFunction::DerivativeAlwaysZero;

  private:
    FunctionP m_p;
};

class DetAMinQI : public IMultiVariableRealValuedFunction
{
  public:
    int GetDomainDimension() const override;
    double Evaluate(std::span<const double> x) const override;
    void Derivative(std::span<const double> x, std::span<double> dfdx) const override;
    bool DerivativeAlwaysZero(int var) const override;
    bool HasDerivative() const override;

    using IMultiVariableRealValuedFunction::Derivative;
    using IMultiVariableRealValuedFunction::DerivativeAlwaysZero;
};

class ComponentAMinQI : public IMultiVariableRealValuedFunction
{
  public:
    ComponentAMinQI(int);
    int GetDomainDimension() const override;
    double Evaluate(std::span<const double> x) const override;
    void Derivative(std::span<const double> x, std::span<double> dfdx) const override;
    bool DerivativeAlwaysZero(int var) const override;
    bool HasDerivative() const override;

    using IMultiVariableRealValuedFunction::Derivative;
    using IMultiVariableRealValuedFunction::DerivativeAlwaysZero;

  private:
    int m_n;
};

class FunctionDetB : public IMultiVariableRealValuedFunction
{
  public:
    FunctionDetB(const IMultiVariableRealValuedFunction &p);
    int GetDomainDimension() const override;
    double Evaluate(std::span<const double> x) const override;
    void Derivative(std::span<const double> x, std::span<double> dfdx) const override;
    bool DerivativeAlwaysZero(int var) const override;
    bool HasDerivative() const override;

    using IMultiVariableRealValuedFunction::Derivative;
    using IMultiVariableRealValuedFunction::DerivativeAlwaysZero;

  private:
    const IMultiVariableRealValuedFunction &m_p;
};

}; // namespace EigenValueSolverSym3x3Utils
