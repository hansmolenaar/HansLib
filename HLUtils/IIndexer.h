#pragma once


#include <initializer_list>

class IIndexer
{
public:
	virtual int ToFlat(std::initializer_list<int> ijk) const = 0;
	~IIndexer(){};
};

