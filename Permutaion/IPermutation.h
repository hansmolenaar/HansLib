#pragma once

class IPermutation
{
public:
   virtual ~IPermutation() = default;
   virtual int operator()(int) const = 0;
   virtual int getCardinality() const = 0;
};