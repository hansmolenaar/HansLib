#include <gtest/gtest.h>

#include "Tree/KdTree.h"


TEST(KdTreeTest, Vertex)
{
   KdTreeVertex<int, 1> vertex(nullptr, nullptr, 1, 2);
}
