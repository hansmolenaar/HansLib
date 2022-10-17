#include "MatrixVector/MatrixKelvinRepr2.h"

#include "Utils/MathConstants.h"
#include "Utils/MessageHandler.h"
#include "MatrixVector/IMatrixKelvinReprUtils.h"
#include "MatrixVector/IndexerKelvinRepr2.h"

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
