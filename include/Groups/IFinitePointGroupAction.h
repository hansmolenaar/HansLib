#pragma once

#include <span>

class IFinitePointGroupAction 
{
public:
   virtual void Transform(std::span<const double>, std::span<double>) const = 0;

   virtual ~IFinitePointGroupAction() {};
};
