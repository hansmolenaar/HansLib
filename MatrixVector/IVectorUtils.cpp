#include "IVectorUtils.h"

#include "IVector.h"


IVector& operator*=(IVector& vec, double factor)
{
	for (int row = 0; row < vec.GetDimension(); ++row)
	{
		vec(row) = factor * vec(row);
	}
	return vec;
}

IVector& operator+=(IVector& vec, const IVector& other)
{
	for (int row = 0; row < vec.GetDimension(); ++row)
	{
		vec(row) += other(row);
	}
	return vec;
}

void SetAll(IVector& vec, double value)
{
	for (int row = 0; row < vec.GetDimension(); ++row)
	{
		vec(row) = value;
	}
}



void Clear(IVector& vec)
{
	SetAll(vec, 0.0);
}

