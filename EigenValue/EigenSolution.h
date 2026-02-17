#pragma once

#include <optional>
#include <span>
#include <vector>

class MatrixKelvinRepr2;
class IRealFunction;

class EigenSolution
{
  public:
    explicit EigenSolution(std::span<const double>);
    EigenSolution(std::span<const double>, const std::vector<std::vector<double>> &);

    // Eigenvalues are sorted increasingly; need not contains all possible eigenvalues
    std::span<const double> getAvailableEigenValues() const;

    bool hasEigenVectors() const;

    // Eigenvector corresponding to n-th eigenvalue
    std::span<const double> getNthEigenVector(size_t) const;

  private:
    std::vector<double> m_eigenValues;
    std::vector<std::vector<double>> m_eigenVectors;
};
