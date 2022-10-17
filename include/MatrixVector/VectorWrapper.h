#pragma once

#include "IVector.h"
#include <span>
class IIndexer;

class VectorWrapper : public IVector
{
public:
	explicit VectorWrapper( std::span<double>);
	int GetDimension() const override;
	double operator() (int)  const override;
	double& operator() (int) override;


private:
	std::span<double> m_data;
};

