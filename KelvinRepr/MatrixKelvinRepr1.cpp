#include "MatrixKelvinRepr1.h"

#include "IndexerKelvinRepr1.h"
#include "Utilities/MessageHandler.h"

namespace
{
	static const IndexerKelvinRepr1 indexer;
}

MatrixKelvinRepr1::MatrixKelvinRepr1() : MatrixKelvinReprBase(indexer)
{
}
