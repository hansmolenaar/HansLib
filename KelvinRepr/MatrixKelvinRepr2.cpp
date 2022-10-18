#include "MatrixKelvinRepr2.h"

#include "HLUtils/MathConstants.h"
#include "HLUtils/MessageHandler.h"
#include "IMatrixKelvinReprUtils.h"
#include "IndexerKelvinRepr2.h"

#include <exception>


namespace
{
	static const IndexerKelvinRepr2 indexer;
}

MatrixKelvinRepr2::MatrixKelvinRepr2() : MatrixKelvinReprBase(indexer)
{
}

MatrixKelvinRepr2 MatrixKelvinRepr2::Create()
{
	return MatrixKelvinRepr2();
}
