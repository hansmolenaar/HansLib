#include "KelvinRepr/MatrixKelvinRepr3.h"

#include "Utils/MathConstants.h"
#include "Utils/MessageHandler.h"
#include "KelvinRepr/IMatrixKelvinReprUtils.h"
#include "KelvinRepr/IndexerKelvinRepr3.h"

#include <exception>


namespace
{
	static const IndexerKelvinRepr3 indexer;
}

MatrixKelvinRepr3::MatrixKelvinRepr3() : MatrixKelvinReprBase(indexer)
{
}

MatrixKelvinRepr3 MatrixKelvinRepr3::CreateEmpty()
{
	return MatrixKelvinRepr3();
}

MatrixKelvinRepr3 MatrixKelvinRepr3::CreateFromVector(std::span<const double> values)
{
	MessageHandler::Assert(values.size() == 6);
	MatrixKelvinRepr3 result;
	std::copy(values.begin(), values.end(),result.m_data.begin());
	return result;
}

MatrixKelvinRepr3 MatrixKelvinRepr3::Create(const IMatrixSymmetric& matrix)
{
	MessageHandler::Assert(matrix.GetDimension() == 3);
	auto result = MatrixKelvinRepr3();
	for (int row = 0; row < 3; ++row)
	{
		for (int col = row; col < 3; ++col)
		{
			result.Set(row, col, matrix(row, col));
		}
	}
	return result;
}

MatrixKelvinRepr3 MatrixKelvinRepr3::CreateIdentity()
{
	auto result = MatrixKelvinRepr3();
	for (int row = 0; row < 3; ++row)
	{
		result.Set(row, row, 1.0);
	}
	return result;
}


MatrixKelvinRepr3 operator*(double factor, const MatrixKelvinRepr3& vec)
{
	MatrixKelvinRepr3 result(vec);
	result *= factor;
	return result;
}


MatrixKelvinRepr3 operator+(const MatrixKelvinRepr3& vec1, const MatrixKelvinRepr3& vec2)
{
	MatrixKelvinRepr3 result(vec1);
	result += vec2;
	return result;
}


MatrixKelvinRepr3 operator-(const MatrixKelvinRepr3& vec1, const MatrixKelvinRepr3& vec2)
{
	MatrixKelvinRepr3 result(vec1);
	result += (-1.0)*vec2;
	return result;
}