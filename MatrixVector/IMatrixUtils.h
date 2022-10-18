#pragma once

#include <span>



class IMatrix;

IMatrix& operator*=(IMatrix& matrix, double factor);
IMatrix& operator+=(IMatrix& matrix, const IMatrix& matrixAdd );

void SetAll(IMatrix& matrix, double value);
void Clear(IMatrix& matrix);

void CheckDimensions(const IMatrix&);
void CheckRowCol(const IMatrix&, int, int);

void MatrixTimesVector(const IMatrix&, std::span<const double>, std::span<double>);
void MatrixTimesMatrix(const IMatrix&, const IMatrix&, IMatrix&);


