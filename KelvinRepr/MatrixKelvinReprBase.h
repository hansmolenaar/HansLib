#pragma once

#include "IMatrixKelvinRepr.h"
#include "HLUtils/IndexerSymmetric.h"
#include "HLUtils/MessageHandler.h"
#include "HLUtils/MathConstants.h"

#include <array>

template<int Dimension>
class MatrixKelvinReprBase : public IMatrixKelvinRepr
{
public:
   int GetDimension() const override { return Dimension; }
   double operator() (int row, int col)  const override { return Get(row, col); }
   double& operator() (int, int) override;
   double Get(int, int) const override;
   void Set(int, int, double) override;

   virtual ~MatrixKelvinReprBase() {};
   static const  int VectorLength = Dimension * (Dimension + 1) / 2;
   std::span<const double> Vector() const override;

   void CopyFrom(const MatrixKelvinReprBase<Dimension>&);
protected:
   explicit MatrixKelvinReprBase(const IIndexer<int>&);

   const IIndexer<int>& m_indexer;
   std::array<double, VectorLength> m_data;
};


template<int Dimension>
MatrixKelvinReprBase< Dimension>::MatrixKelvinReprBase(const IIndexer<int>& indexer) : m_indexer(indexer)
{
   std::fill_n(m_data.begin(), VectorLength, 0.0);
}


template<int Dimension>
double& MatrixKelvinReprBase<Dimension>::operator() (int, int)
{
   throw std::exception("Don't call me, use Get(int,int) instead");
}


template<int Dimension>
std::span<const double> MatrixKelvinReprBase<Dimension>::Vector() const
{
   return m_data;
}

template<int Dimension>
double MatrixKelvinReprBase< Dimension>::Get(int row, int col) const
{
   const double factor = (row == col ? 1 : MathConstants::SQRT1_2);
   return factor * m_data[m_indexer.ToFlat({ row, col })];
}

template<int Dimension>
void MatrixKelvinReprBase< Dimension>::Set(int row, int col, double val)
{
   const double factor = (row == col ? 1 : MathConstants::SQRT2);
   m_data[m_indexer.ToFlat({ row, col })] = factor * val;
}

template<int Dimension>
void MatrixKelvinReprBase< Dimension>::CopyFrom(const MatrixKelvinReprBase<Dimension>& source)
{
   std::copy(source.m_data.begin(), source.m_data.end(), m_data.begin());
}

