#pragma once

class IGroupTable;

template <typename TElement>
class IFiniteGroup
{
public:
   virtual const IGroupTable& GroupTable() const = 0;
   virtual TElement GetElement(int) const = 0;

   virtual ~IFiniteGroup() {};
};

