#include "KelvinRepr/IndexerKelvinRepr1.h"

#include "Utils/MessageHandler.h"


IndexerKelvinRepr1::IndexerKelvinRepr1() 
{
	// Empty
}

int IndexerKelvinRepr1::ToFlat(std::initializer_list<int> ijk) const
{
	MessageHandler::Assert(2 == ijk.size());
	const int row = *ijk.begin();
	const int col = *(ijk.begin() + 1);
	MessageHandler::Assert(row == 0 && col == 0);
	return 0;
}