#pragma once

#include <vector>

class IMinimization
{
public:
   virtual ~IMinimization() noexcept = default;
   virtual double getEvalMin() = 0;
   virtual std::vector<double> getArgMin() const = 0;
};