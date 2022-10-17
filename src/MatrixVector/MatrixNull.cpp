#include "MatrixVector/MatrixNull.h"

#include <exception>

int MatrixNull::GetRowDimension() const
{
	return 0;
}


int MatrixNull::GetColDimension() const
{
	return 0;
}


double MatrixNull::operator() (int row, int col)  const
{
	throw std::exception("Do not call me");
}

double& MatrixNull::operator() (int row, int col)
{
	throw std::exception("Do not call me");
}
