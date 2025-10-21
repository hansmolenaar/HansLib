#include "NodalBasis1D.h"
#include "MyException.h"

#include <cmath>

NodalBasis1D::NodalBasis1D(std::unique_ptr<TartanGrid<double, 1>> &&grid, std::vector<double> &&coefficients)
    : m_grid(std::move(grid)), m_coefficients(std::move(coefficients))
{
}

double NodalBasis1D::Evaluate(double x) const
{
    const auto cellId = m_grid->locatePointInCell(Point1{x});
    if (cellId == CellIndexInvalid)
        throw MyException("NodalBasis1D::Evaluate() point outside grid specified");
    const auto &geometry = m_grid->getGeometry();
    const auto x0 = geometry.getPoint(cellId).at(0);
    const auto x1 = geometry.getPoint(cellId + 1).at(0);
    const double pos = (x - x0) / (x1 - x0);
    const auto result = std::lerp(m_coefficients.at(cellId), m_coefficients.at(cellId + 1), pos);
    return result;
}

bool NodalBasis1D::HasDerivative() const
{
    return false;
}

bool NodalBasis1D::DerivativeAlwaysZero(int eqn, int var) const
{
    throw MyException("NodalBasis1D::DerivativeAlwaysZero should not come here");
}

double NodalBasis1D::Derivative(double x) const
{
    throw MyException("NodalBasis1D::Derivative should not come here");
}

bool NodalBasis1D::IsNonConstant() const
{
    return str::any_of(m_coefficients, [](const double val) { return val != 0.0; });
}