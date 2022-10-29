#pragma once

class IFiniteGroup
{
public:
   virtual ~IFiniteGroup() = default;
   virtual int getOrder() const = 0;
   // 0 must be identity
   virtual int inverse(int) const = 0;
   virtual int operator()(int, int) const = 0;
};
