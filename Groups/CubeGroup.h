#pragma once

#include "IFiniteGroup.h"
#include "Defines.h"
#include "Permutation.h"

//                   6 ------ 7
//                  /        /
//                 4 ------ 5
//                  
//                   2 ------ 3
//                  /        /
//                 0 ------ 1
//
// Edges: (0,1), (0,2), (0,4), (1,3), (1,5), (2,3), (2,6), (3,7), (4, 5), (4, 6), (5,7), (6,7)

class CubeGroup : public IFiniteGroup
{
public:
   using Vertex = unsigned short;
   static constexpr Vertex numVertices = 8;

   //static std::unique_ptr<CubeGroup> Create(int);

   int getOrder() const override;
   GroupElement getIdentity() const override;
   GroupElement getInverse(GroupElement) const override;
   GroupElement operator()(GroupElement, GroupElement) const override;

   static int getDistance(Vertex v1, Vertex v2);
   static std::vector<Permutation> getRotations();
   static std::vector<Permutation> getReflections();
   static std::vector<Permutation> getSymmetries();
   static std::vector<Permutation> getGroupSymmetries();
   static bool isIsometry(const Permutation& permutation);

private:
   std::unique_ptr<IFiniteGroup> m_groupTable;
};