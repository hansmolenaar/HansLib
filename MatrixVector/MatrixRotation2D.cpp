#include "MatrixVector/MatrixRotation2D.h"
#include "MatrixVector/IMatrixUtils.h"
#include "Utils/MessageHandler.h"

#include <cmath>


#if 0

class MatrixRotation2D : public IMatrixSquare, IFinitePointGroupAction<2>
{
public:
   MatrixRotation2D(double);
   int GetRowDimension() const override;
   int GetColDimension() const override;

   double operator() (int, int)  const override;
   double& operator() (int, int) override;

   std::array < double, Dimension > operator()(std::array < double, Dimension >) const override;
};
#endif

int MatrixRotation2D::GetDimension() const
{
   return 2;
}


double MatrixRotation2D::operator() (int row, int col)  const
{
   return m_matrix(row, col);
}

MatrixRotation2D::MatrixRotation2D(double angle) :
   m_matrix(2,2)
{
   const double cosx = std::cos(angle);
   const double sinx = std::sin(angle);
   m_matrix(0, 0) = cosx;
   m_matrix(1, 1) = cosx;
   m_matrix(0, 1) = -sinx;
   m_matrix(1, 0) = sinx;
}

void MatrixRotation2D::Transform(std::span<const double> vin, std::span<double> vout) const
{
   MatrixTimesVector(m_matrix, vin, vout);
}
