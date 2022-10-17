#pragma once




class IVector
{
public:
	virtual int GetDimension() const = 0;
	virtual double operator() (int)  const = 0;
	virtual double& operator() (int)  = 0;

	~IVector() {};
};

