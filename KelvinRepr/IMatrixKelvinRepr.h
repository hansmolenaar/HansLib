#pragma once

#include "MatrixVector/IMatrixSymmetric.h"
#include <span>

class IMatrixKelvinRepr : public IMatrixSymmetric
{
public:
	// throw exception on: double& operator() (int, int)
	// use Set() instead
	virtual double Get(int, int) const = 0;
	virtual void Set(int, int, double) = 0;
	virtual std::span<const double> Vector() const = 0;
	virtual ~IMatrixKelvinRepr() {};
};
