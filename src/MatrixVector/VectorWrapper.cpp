
#include "MatrixVector/VectorWrapper.h"

VectorWrapper::VectorWrapper( std::span<double> data) :
	 m_data(data)
{
}

int VectorWrapper::GetDimension() const
{
	return m_data.size();
}


double VectorWrapper::operator() (int n)  const
{
	return m_data[n];
}


double&  VectorWrapper::operator() (int n)
{
	return m_data[n];
}
