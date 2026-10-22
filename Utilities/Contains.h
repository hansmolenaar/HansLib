#pragma once

#include <iterator>

namespace Utilities
{

template <typename InputItr, class T = typename std::iterator_traits<InputItr>::value_type>
bool Contains(InputItr b, InputItr e, const T &v)
{
   return std::find(b, e, v) != e;
}

template <typename C, typename V> bool Contains(const C &container, V value)
{
   return std::ranges::find(container, value) != container.end();
}

} // namespace Utilities
