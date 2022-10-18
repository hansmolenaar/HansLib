#pragma once


#include "IVector.h"
#include "HLUtils/MessageHandler.h"

#include <array>
#include <memory>
#include <numeric>
#include <span>
#include <cmath>

template<int Dimension>
class UnitVector : public IVector
{
public:
   static std::unique_ptr<UnitVector<Dimension>> TryCreate(std::span<const double>);
   int GetDimension() const override;
   double operator() (int)  const override;
   double& operator() (int) override;

   virtual ~UnitVector();
private:
   UnitVector(std::span<const double>);

   std::array<double, Dimension> m_data;
};

typedef  UnitVector<2> UnitVector2;
typedef  UnitVector<3> UnitVector3;


template<int Dimension>
UnitVector<Dimension>::UnitVector(std::span<const double> data)
{
   std::copy(data.begin(), data.end(), m_data.begin());
}

template<int Dimension>
UnitVector<Dimension>::~UnitVector()
{

}


template<int Dimension>
int UnitVector<Dimension>::GetDimension() const
{
   return Dimension;
}

template<int Dimension>
double UnitVector<Dimension>::operator() (int n)  const
{
   return m_data.at(n);
}

template<int Dimension>
double&  UnitVector<Dimension>::operator() (int n)
{
   throw std::exception("Don't call me, use const version instead");
}


// Returns null pointer in case of failure
template<int Dimension>
std::unique_ptr<UnitVector<Dimension>> UnitVector<Dimension>::TryCreate(std::span<const double> vec)
{
   std::unique_ptr<UnitVector<Dimension>> result;
   MessageHandler::Assert(vec.size() == Dimension);
   const double nrm2 = std::inner_product(vec.begin(), vec.end(), vec.begin(), 0.0);
   const double nrm = std::sqrt(nrm2);

   if (nrm != 0.0)
   {
      std::array<double, Dimension>  data;
      for (int n = 0; n < Dimension; ++n)
      {
         data[n] = vec[n] / nrm;
      }
      result.reset(new UnitVector<Dimension>(data));
   }
   return result;
}
