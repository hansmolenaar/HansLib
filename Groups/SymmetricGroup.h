#pragma once

#include "IFiniteGroup.h"

#include <memory>

class SymmetricGroup
{
  public:
    static std::unique_ptr<IFiniteGroup> Create(GroupElement size);
};
