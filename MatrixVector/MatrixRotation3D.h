#pragma once


#include "IMatrixRotation.h"
#include "IFinitePointGroupAction.h"
#include "MatrixDense.h"
#include "MatrixVector/UnitVector.h"


class MatrixRotation3D : public IMatrixRotation
{
public:
   static MatrixRotation3D Create(const UnitVector3&, double);
	int GetDimension() const override;

	double operator() (int, int)  const override;

   void Transform(std::span<const double>, std::span<double>) const;
private:
   MatrixRotation3D(const UnitVector3&, double);

   MatrixDense   m_matrix;
};
