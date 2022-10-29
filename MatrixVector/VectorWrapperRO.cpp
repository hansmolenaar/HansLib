
#include "MatrixVector/VectorWrapperRO.h"

#include <exception>

VectorWrapperRO::VectorWrapperRO(std::span<const double> data) :
   m_data(data)
{
}

int VectorWrapperRO::GetDimension() const
{
   return static_cast<int>(m_data.size());
}


double VectorWrapperRO::operator() (int n)  const
{
   return m_data[n];
}


double& VectorWrapperRO::operator() (int n)
{
   throw std::exception("Come on, I am ReadOnly");
}
