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
    const size_t matrixDim = eigenVectors.front().size();
    for (size_t n : Iota::GetRange(numEigenValues))
    {
        MyAssert(eigenVectors.at(n).size() == matrixDim);
    }

    std::vector<size_t> indices(Iota::GenerateVector(numEigenValues));
    str::sort(indices, [&eigenValues](size_t lhs, size_t rhs) { return eigenValues[lhs] < eigenValues[rhs]; });
    for (auto n : indices)
    {
        m_eigenValues.emplace_back(eigenValues[n]);
        m_eigenVectors.emplace_back(eigenVectors[n]);
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
    return m_eigenVectors.at(n);
}
