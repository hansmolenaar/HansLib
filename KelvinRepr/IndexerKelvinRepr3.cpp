#include "IndexerKelvinRepr3.h"

#include "HLUtils/MessageHandler.h"

#include <algorithm>


//     0 5 4
//     5 1 3
//     4 3 2

IndexerKelvinRepr3::IndexerKelvinRepr3()
{
   // Empty
}

int IndexerKelvinRepr3::ToFlat(std::initializer_list<int> ijk) const
{
   MessageHandler::Assert(2 == ijk.size());
   const int row = *ijk.begin();
   const int col = *(ijk.begin() + 1);

   if (row == col)
   {
      MessageHandler::Assert(row >= 0 && row < 3);
      return row;
   }
   else
   {
      const int minRC = std::min(row, col);
      const int maxRC = std::max(row, col);
      MessageHandler::Assert(minRC >= 0 && maxRC < 3);
      if (minRC == 0)
      {
         return (maxRC == 1 ? 5 : 4);
      }
      else
      {
         return 3;
      }
   }
   MessageHandler::Assert(false);
   return -1;
}


void IndexerKelvinRepr3::ToRowCol(const int flat, int&row, int&col)
{
   switch (flat)
   {
   case 0:
   case 1:
   case 2:
      row = flat;
      col = flat;
      break;
   case 3:
      row = 1;
      col = 2;
      break;
   case 4:
      row = 0;
      col = 2;
      break;
   case 5:
      row = 0;
      col = 1;
      break;
   default:
      MessageHandler::Assert(false);
      break;
   }
}