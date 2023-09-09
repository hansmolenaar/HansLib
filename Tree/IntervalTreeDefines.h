#pragma once


namespace IntervalTree
{
   using Level = int;

   template<int N>
   constexpr int NumKids = 1 << N;
}

