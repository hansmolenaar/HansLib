#pragma once

class IMatrixKelvinRepr;

IMatrixKelvinRepr &operator*=(IMatrixKelvinRepr &matrix, double factor);
IMatrixKelvinRepr &operator+=(IMatrixKelvinRepr &matrix, const IMatrixKelvinRepr &matrixAdd);

void SetAll(IMatrixKelvinRepr &matrix, double value);
void Clear(IMatrixKelvinRepr &matrix);
