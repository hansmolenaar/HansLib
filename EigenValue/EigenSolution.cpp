#include "EigenSolution.h"

#include "Defines.h"
#include "Iota.h"
#include "MyAssert.h"

using namespace Utilities;

EigenSolution::EigenSolution(std::span<const double> eigenValues)
    : m_eigenValues(eigenValues.begin(), eigenValues.end())
{
    str::sort(m_eigenValues);
}

EigenSolution::EigenSolution(std::span<const double> eigenValues, const std::vector<std::vector<double>> &eigenVectors)
{
    MyAssert(!eigenValues.empty());
    const size_t numEigenValues = eigenValues.size();
    MyAssert(numEigenValues == eigenVectors.size());
    m_matrixDimension = eigenVectors.front().size();
    m_eigenVectors.resize(m_matrixDimension * numEigenValues);
    for (size_t n : Iota::GetRange(numEigenValues))
    {
        MyAssert(eigenVectors.at(n).size() == m_matrixDimension);
    }

    std::vector<size_t> indices(Iota::GenerateVector(numEigenValues));
    str::sort(indices, [&eigenValues](size_t lhs, size_t rhs) { return eigenValues[lhs] < eigenValues[rhs]; });
    auto itr = m_eigenVectors.begin();
    for (auto n : indices)
    {
        m_eigenValues.emplace_back(eigenValues[n]);
        str::copy(eigenVectors[n], itr);
        itr += m_matrixDimension;
    }
}

std::span<const double> EigenSolution::getAvailableEigenValues() const
{
    return m_eigenValues;
}

bool EigenSolution::hasEigenVectors() const
{
    return !m_eigenVectors.empty();
}

std::span<const double> EigenSolution::getNthEigenVector(size_t n) const
{
    MyAssert(n < m_eigenValues.size());
    return {m_eigenVectors.data() + n * m_matrixDimension, m_matrixDimension};
}

std::ostream &operator<<(std::ostream &os, const EigenSolution &solution)
{
    const auto eigenValues = solution.getAvailableEigenValues();

    if (!solution.hasEigenVectors())
    {
        os << "Eigenvalues: ";
        bool first = true;
        for (double ev : eigenValues)
        {
            if (!first)
            {
                os << ", ";
            }
            first = false;
            os << ev;
        }
        os << "\n";
        return os;
    }

    os << "Eigenvalues and vectors:\n";
    for (size_t n : Iota::GetRange(eigenValues.size()))
    {
        os << "   " << eigenValues[n] << " -> ( ";
        bool first = true;
        for (double ev : solution.getNthEigenVector(n))
        {
            if (!first)
            {
                os << ", ";
            }
            first = false;
            os << ev;
        }
        os << " )\n";
    }

    return os;
}
