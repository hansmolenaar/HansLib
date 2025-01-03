#include "IndexerKelvinRepr3.h"

#include "MyAssert.h"

#include <algorithm>


//     0 5 4
//     5 1 3
//     4 3 2

IndexerKelvinRepr3::IndexerKelvinRepr3()
{
   // Empty
}

size_t IndexerKelvinRepr3::ToFlat(std::initializer_list<int> ijk) const
{
   Utilities::MyAssert(2 == ijk.size());
   const int row = *ijk.begin();
   const int col = *(ijk.begin() + 1);

   if (row == col)
   {
      Utilities::MyAssert(row >= 0 && row < 3);
      return row;
   }
   else
   {
      const int minRC = std::min(row, col);
      const int maxRC = std::max(row, col);
      Utilities::MyAssert(minRC >= 0 && maxRC < 3);
      if (minRC == 0)
      {
         return (maxRC == 1 ? 5 : 4);
      }
      else
      {
         return 3;
      }
   }
   Utilities::MyAssert(false);
   return -1;
}


void IndexerKelvinRepr3::ToRowCol(const size_t flat, int& row, int& col)
{
   switch (flat)
   {
   case 0:
   case 1:
   case 2:
      row = static_cast<int>(flat);
      col = static_cast<int>(flat);
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
      Utilities::MyAssert(false);
      break;
   }
}