#pragma once

#include "IMatrixSquare.h"
#include "MatrixDense.h"
#include "Groups/IFinitePointGroupAction.h"

class IMatrixKelvinRepr;

class IMatrixRotation : public IMatrixSquare
{
public:
   double& operator() (int, int)   override;
   using  IMatrix::operator();
   virtual ~IMatrixRotation() = default;

   static bool IsRotation(const IMatrixSquare& matrix, double tol = 1.0e-12);
   void RotateTensor(const IMatrixKelvinRepr&, IMatrixKelvinRepr&) const;
};
