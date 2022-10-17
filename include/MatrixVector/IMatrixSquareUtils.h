#pragma once


class IMatrixSquare;

double GetDeterminant(const IMatrixSquare& matrix);
double GetDeterminantDerivative(const IMatrixSquare& m, int row, int col);

double GetTrace(const IMatrixSquare& matrix);

bool IsOrthogonal(const IMatrixSquare& matrix, double tol = 1.0e-12);
