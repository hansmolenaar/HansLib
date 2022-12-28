#pragma once

class Pow2
{
public:
   size_t operator()(size_t n) const { return static_cast<size_t>(1) << n; }
};
