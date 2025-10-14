#include "IMatrixUtils.h"
#include "MatrixRotation3D.h"

int MatrixRotation3D::GetDimension() const
{
   return 3;
}


double MatrixRotation3D::operator() (int row, int col)  const
{
   return m_matrix(row, col);
}


MatrixRotation3D::MatrixRotation3D(const UnitVector3& axis, double angle) :
   m_matrix(3, 3)
{
   double ux = axis[0];
   double uy = axis[1];
   double uz = axis[2];

   double c = std::cos(angle);
   double s = std::sin(angle);
   double cc = 1 - c;
   m_matrix(0, 0) = c + ux * ux * cc;
   m_matrix(1, 1) = c + uy * uy * cc;
   m_matrix(2, 2) = c + uz * uz * cc;

   m_matrix(0, 1) = ux * uy * cc - uz * s;
   m_matrix(1, 0) = ux * uy * cc + uz * s;

   m_matrix(0, 2) = ux * uz * cc + uy * s;
   m_matrix(2, 0) = ux * uz * cc - uy * s;

   m_matrix(1, 2) = uy * uz * cc - ux * s;
   m_matrix(2, 1) = uy * uz * cc + ux * s;
}

MatrixRotation3D MatrixRotation3D::Create(const UnitVector3& axis, double angle)
{
   return  MatrixRotation3D(axis, angle);
}

void MatrixRotation3D::Transform(std::span<const double> vin, std::span<double> vout) const
{
   MatrixTimesVector(m_matrix, vin, vout);
}