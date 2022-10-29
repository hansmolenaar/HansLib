#pragma once


#include <initializer_list>

template <typename I>
class IIndexer
{
public:
	virtual	~IIndexer() = default;
	virtual int ToFlat(std::initializer_list<I> ijk) const = 0;
};

