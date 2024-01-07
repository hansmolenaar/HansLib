#pragma once

#include "IUniquePointCollection.h"

template<typename T, int N>
class IDynamicUniquePointCollection : public IUniquePointCollection<T, N>
{
	virtual PointIndex addIfNew(const Point<double, N>& point) = 0;
	virtual void deletePoint(PointIndex pointId) = 0;
	//virtual void movePoint(PointIndex pointId, const Point<double, N>& newLocation) = 0;
};