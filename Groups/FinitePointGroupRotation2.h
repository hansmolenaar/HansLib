#pragma once

#include "IFinitePointGroup.h"
#include "MatrixVector/MatrixRotation2D.h"

class FinitePointGroupRotation2 : public IFinitePointGroup
{
public:
    static const FinitePointGroupRotation2& GetInstance(int);

   const IGroupTable& GroupTable() const override;
   const IFinitePointGroupAction& GetElement(int) const override;
   const MatrixRotation2D& GetRotation(int) const;
private:
   FinitePointGroupRotation2(int);
   std::vector<MatrixRotation2D> m_rotations;

};
