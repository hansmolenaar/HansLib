#include "GroupTable.h"
#include "Utilities/MessageHandler.h"
#include "Utilities/ToString.h"

#include <cmath>


GroupTable::GroupTable(const std::vector<std::tuple<GroupElement, GroupElement, GroupElement>>& in1_in2_out)
{
   int dim =static_cast<int>( std::round(std::sqrt(static_cast<double>(in1_in2_out.size()))));
   if (dim * dim != in1_in2_out.size())
   {
      MessageHandler::Error("GroupTable::GroupTable() size incorrect: " + ToString(in1_in2_out.size()));
   }
}