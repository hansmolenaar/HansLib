#pragma once
#include "IMinimization.h"
#include <vector>

enum IIterativeMinimizationStatus  { Converged, NotConverged, MaxIterExceeded };

class IIterativeMinimization : public IMinimization
{
public:
    // Returns if a better approximation has been found
	virtual bool iterate() = 0;
	virtual int getNumIterations() const = 0;
};