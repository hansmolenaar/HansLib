#pragma once

#include "MyAssert.h"

class IMatrix
{
public:
   virtual int GetRowDimension() const = 0;
   virtual int GetColDimension() const = 0;
   virtual double operator() (int, int)  const = 0;
   virtual double& operator() (int, int) = 0;

   inline void CopyFrom(const IMatrix& other);

   virtual ~IMatrix() {};
};


void IMatrix::CopyFrom(const IMatrix& other)
{
   if (this != &other) { // self-assignment check expected
      const int nRows = GetRowDimension();
      const int nCols = GetColDimension();
      Utilities::MyAssert(other.GetRowDimension() == nRows);
      Utilities::MyAssert(other.GetColDimension() == nCols);
      for (int r = 0; r < nRows; ++r)
      {
         for (int c = 0; c < nCols; ++c)
         {
            (*this)(r, c) = other(r, c);
         }
      }
   }
}
