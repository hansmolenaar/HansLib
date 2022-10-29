
#include "MatrixVector/Vector.h"

Vector::Vector(int dim) : m_data(dim)
{

}

int Vector::GetDimension() const
{
   return static_cast<int>(m_data.size());
}

double Vector::operator() (int n)  const
{
   return m_data.at(n);
}

double& Vector::operator() (int n)
{
   return m_data.at(n);
}

double* Vector::data()
{
   return m_data.data();
}