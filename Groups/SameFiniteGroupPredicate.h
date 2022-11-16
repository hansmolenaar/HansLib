#pragma once

#include "Groups/IFiniteGroup.h"

struct SameFiniteGroupPredicate
{
   const IFiniteGroup& group;
   bool operator()(const IFiniteGroup& ) const;
};