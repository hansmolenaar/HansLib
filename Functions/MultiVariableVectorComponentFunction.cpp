#include "MultiVariableVectorComponentFunction.h"
#include "MyAssert.h"
#include <map>
#include <memory>

namespace
{
std::map<std::tuple<int, int>, std::unique_ptr<MultiVariableVectorComponentFunction>> s_instances;
};

MultiVariableVectorComponentFunction::MultiVariableVectorComponentFunction(int ind, int dim) : m_index(ind), m_dim(dim)
{
    Utilities::MyAssert(ind >= 0);
    Utilities::MyAssert(ind < dim);
}

const MultiVariableVectorComponentFunction &MultiVariableVectorComponentFunction::Instance(int ind, int dim)
{
    const std::tuple<int, int> id(ind, dim);
    if (s_instances.find(id) == s_instances.end())
    {
        s_instances[id] =
            std::unique_ptr<MultiVariableVectorComponentFunction>(new MultiVariableVectorComponentFunction(ind, dim));
    }
    return *s_instances[id];
}

int MultiVariableVectorComponentFunction::GetDomainDimension() const
{
    return m_dim;
}

bool MultiVariableVectorComponentFunction::DerivativeAlwaysZero(int var) const
{
    Utilities::MyAssert(var >= 0 && var < m_dim);
    return var != m_index;
}

bool MultiVariableVectorComponentFunction::HasDerivative() const
{
    return true;
}

double MultiVariableVectorComponentFunction::Evaluate(std::span<const double> x) const
{
    Utilities::MyAssert(x.size() == GetDomainDimension());
    return x[m_index];
}

void MultiVariableVectorComponentFunction::Derivative(std::span<const double> x, std::span<double> dfdx) const
{
    const int dim = GetDomainDimension();

    Utilities::MyAssert(x.size() == GetDomainDimension());
    Utilities::MyAssert(dfdx.size() == dim);

    std::fill(dfdx.begin(), dfdx.end(), 0.0);
    dfdx[m_index] = 1.0;
}