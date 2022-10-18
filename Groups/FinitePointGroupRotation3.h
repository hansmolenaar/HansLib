#pragma once

#include "IFinitePointGroup.h"
#include "MatrixVector/MatrixRotation3D.h"

class FinitePointGroupRotation3 : public IFinitePointGroup
{
public:
     FinitePointGroupRotation3(int, const UnitVector3&);

   const IGroupTable& GroupTable() const override;
   const IFinitePointGroupAction& GetElement(int) const override;
   const MatrixRotation3D& GetRotation(int) const;
private:
   std::vector<MatrixRotation3D> m_rotations;

};
