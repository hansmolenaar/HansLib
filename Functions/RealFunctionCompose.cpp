#include "RealFunctionCompose.h"

#include "IMatrixUtils.h"
#include "IRealFunctionUtils.h"
#include "MatrixDense.h"
#include "MyAssert.h"

// f: R^k-> R^n
// g: R^m -> R^k
// f*g -> R^m -> R^n

RealFunctionCompose::RealFunctionCompose(const IRealFunction &f, const IRealFunction &g) : m_f(f), m_g(g)
{
    Utilities::MyAssert(m_f.GetDomainDimension() == m_g.GetRangeDimension());
}

int RealFunctionCompose::GetRangeDimension() const
{
    return m_f.GetRangeDimension();
}

int RealFunctionCompose::GetDomainDimension() const
{
    return m_g.GetDomainDimension();
}

bool RealFunctionCompose::HasDerivative() const
{
    return true;
}

void RealFunctionCompose::Derivative(std::span<const double> x, IMatrix &dfdx) const
{
    const int M = m_g.GetDomainDimension();
    const int K = m_g.GetRangeDimension();
    const int N = m_f.GetRangeDimension();

    Utilities::MyAssert(dfdx.GetRowDimension() == N);
    Utilities::MyAssert(dfdx.GetColDimension() == M);

    std::vector<double> z(K);
    m_g.Evaluate(x, z);

    MatrixDense df(N, K);
    m_f.Derivative(z, df);

    MatrixDense dg(K, M);
    m_g.Derivative(x, dg);

    Clear(dfdx);
    for (int m = 0; m < M; ++m)
    {
        for (int n = 0; n < N; ++n)
        {
            if (!DerivativeAlwaysZero(n, m))
            {
                double val = 0;
                for (int k = 0; k < K; ++k)
                {
                    val += df(n, k) * dg(k, m);
                }
                dfdx(n, m) = val;
            }
        }
    }
}

void RealFunctionCompose::Evaluate(std::span<const double> x, std::span<double> y) const
{
    std::vector<double> z(m_g.GetRangeDimension());
    m_g.Evaluate(x, z);
    m_f.Evaluate(z, y);
}

bool RealFunctionCompose::DerivativeAlwaysZero(int eqn, int var) const
{
    CheckIndices(*this, eqn, var);
    for (int k = 0; k < m_g.GetRangeDimension(); ++k)
    {
        if (!m_g.DerivativeAlwaysZero(k, var) && !m_f.DerivativeAlwaysZero(eqn, k))
        {
            return false;
        }
    }
    return true;
}
