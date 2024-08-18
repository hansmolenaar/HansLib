#pragma once

#include "IAnnModel.h"

namespace ML
{
   namespace IAnnModelUtils
   {
      void checkDimensions(const ML::IAnnModel& model);
      std::vector<size_t> getLayerDimensions(const ML::IAnnModel& model);
   };
}
