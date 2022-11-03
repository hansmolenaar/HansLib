#pragma once


#include "IMatrixRotation.h"
#include "MatrixDense.h"
#include "Groups/IFinitePointGroupAction.h"

#include <map>

class MatrixRotation2D : public IMatrixRotation
{
public:
   MatrixRotation2D(double); 
	int GetDimension() const override;

	double operator() (int, int)  const override;

   void Transform(std::span<const double>, std::span<double>) const;

private:
   MatrixDense   m_matrix;
};
