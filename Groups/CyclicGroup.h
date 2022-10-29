#pragma once

#include "IFiniteGroup.h"
#include "HLUtils/Defines.h"


class CyclicGroup : public IFiniteGroup
{
public:
   static std::unique_ptr<CyclicGroup> Create(int);

    int getOrder() const override;
   // 0 must be identity
    int inverse(int) const override;
    int operator()(int, int) const override;

private:
   CyclicGroup(int);
   int m_order;
};

