#pragma once

#include <span>
#include "NonLinearSolverStatus.h"

class INonLinearSolverStrategy
{
public:
	virtual NonLinearSolverStatus GetStatus(int iter, std::span<const  double> rsd) = 0;
	virtual void Reset() = 0;
	virtual ~INonLinearSolverStrategy() {}
};
