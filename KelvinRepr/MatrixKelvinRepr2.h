#pragma once

#include "MatrixKelvinReprBase.h"

class MatrixKelvinRepr2 : public MatrixKelvinReprBase<2>
{
  public:
    static MatrixKelvinRepr2 Create();

  private:
    MatrixKelvinRepr2();
};
