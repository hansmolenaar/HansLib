#pragma once

#include "IFiniteGroup.h"

#include <memory>

class AlternatingGroup
{
  public:
    static std::unique_ptr<IFiniteGroup> Create(GroupElement size);
};
