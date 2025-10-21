#pragma once

class IVector;

IVector &operator*=(IVector &matrix, double factor);
IVector &operator+=(IVector &thisVector, const IVector &other);

void SetAll(IVector &, double value);
void Clear(IVector &);
