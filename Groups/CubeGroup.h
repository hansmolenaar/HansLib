#pragma once

#include "Defines.h"
#include "IFiniteGroup.h"
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
    using Vertex = Permutation::Entry;
    static constexpr Vertex numVertices = 8;

    static std::pair<std::unique_ptr<IFiniteGroup>, std::vector<Permutation>> Create();

    GroupElement getOrder() const override;
    GroupElement getIdentity() const override;
    GroupElement getInverse(GroupElement) const override;
    GroupElement operator()(GroupElement, GroupElement) const override;

    static int getDistance(Vertex v1, Vertex v2);
    static std::vector<Permutation> getRotations();
    static std::vector<Permutation> getReflections();
    static std::vector<Permutation> getReflectionsIjk();
    static std::vector<Permutation> getSymmetries();
    static std::vector<Permutation> getGroupSymmetries();
    static bool isIsometry(const Permutation &permutation);

  private:
    std::unique_ptr<IFiniteGroup> m_groupTable;
};