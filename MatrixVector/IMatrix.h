#pragma once

#include "MyAssert.h"

#include <span>
#include <vector>

class IMatrix
{
  public:
    virtual ~IMatrix() = default;

    virtual int GetRowDimension() const = 0;
    virtual int GetColDimension() const = 0;
    virtual double get(int, int) const = 0;
    virtual void set(int, int, double) = 0;
    virtual void timesVector(std::span<const double>, std::span<double>) const = 0;

    inline void CopyFrom(const IMatrix &other);
    inline void add(int, int, double);
};

void IMatrix::CopyFrom(const IMatrix &other)
{
    if (this != &other)
    { // self-assignment check expected
        const int nRows = GetRowDimension();
        const int nCols = GetColDimension();
        Utilities::MyAssert(other.GetRowDimension() == nRows);
        Utilities::MyAssert(other.GetColDimension() == nCols);
        for (int r = 0; r < nRows; ++r)
        {
            for (int c = 0; c < nCols; ++c)
            {
                set(r, c, other.get(r, c));
            }
        }
    }
}

void IMatrix::add(int row, int col, double value)
{
    set(row, col, get(row, col) + value);
}
