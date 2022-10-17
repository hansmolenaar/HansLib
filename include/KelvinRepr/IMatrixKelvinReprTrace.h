#pragma once

#include "Functions/MultiVariableRealValuedFunction.h"

class IMatrixKelvinRepr;

class IMatrixKelvinReprTrace : public MultiVariableRealValuedFunction
{
public:
	explicit IMatrixKelvinReprTrace(int geomdim);
};
