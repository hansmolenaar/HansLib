#pragma once

#include "MultiVariableRealValuedFunction.h"

class IMatrixKelvinRepr;

class IMatrixKelvinReprTrace : public MultiVariableRealValuedFunction
{
public:
	explicit IMatrixKelvinReprTrace(int geomdim);
};
