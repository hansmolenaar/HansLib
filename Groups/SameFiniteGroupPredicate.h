#pragma once

#include "IFiniteGroup.h"

struct SameFiniteGroupPredicate
{
   const IFiniteGroup& group;
   bool operator()(const IFiniteGroup&) const;
};