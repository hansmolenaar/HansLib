#pragma once

#include "IGroupTable.h"

class GroupTableCyclic : public IGroupTable
{
public:
   static const GroupTableCyclic& Instance(int order);
   int Order() const override;
   int Identity() const override;
   int Inverse(int) const override;
   int operator()(int, int) const override;

private:
   GroupTableCyclic(int);

   int m_order;
};
