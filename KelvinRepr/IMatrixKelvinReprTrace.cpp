#include "IMatrixKelvinReprTrace.h"

#include "IMatrixKelvinRepr.h"
#include "IMatrix.h"
#include "IMatrixUtils.h"
#include "MyAssert.h"

#include <algorithm>
#include <numeric>
#include <vector>
#include <span>


namespace {
	constexpr int TriangleNumber(int dim)
	{
		return dim * (dim + 1) / 2;
	}

	std::vector<bool> GenerateActive(int geomdim)
	{
		std::vector<bool> result(TriangleNumber(geomdim), false);
		std::fill_n(result.begin(), geomdim, true);
		return result;
	}

	double Eval(int geomdim, std::span<const double>x)
	{
		Utilities::MyAssert(TriangleNumber(geomdim) == x.size());
		return std::accumulate(x.begin(), x.begin() + geomdim, 0.0);
	}


	void Deriv(int geomdim, std::span<const double>x, std::span< double> df)
	{
		const int siz = TriangleNumber(geomdim); 
		Utilities::MyAssert(siz == x.size() && siz == df.size()); 
		std::fill(df.begin() + geomdim, df.end(), 0.0); 
		std::fill_n(df.begin(), geomdim, 1.0);
	}
}

IMatrixKelvinReprTrace::IMatrixKelvinReprTrace(int geomdim) :
	MultiVariableRealValuedFunction(
		GenerateActive(geomdim),
		[geomdim](std::span<const double>x) {return Eval(geomdim, x); },
		[geomdim](std::span<const double>x, std::span< double> df) {Deriv(geomdim, x, df); }
	)
{
}
