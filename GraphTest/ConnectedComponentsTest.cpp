#include <gtest/gtest.h>

#include "GraphDefines.h"

TEST(ConnectedComponentsTest, Basics)
{
   using MyGraph = Graph<int,int>;
   MyGraph c;  
   boost::add_vertex (c); 
   boost::add_vertex (c); 
   boost::add_vertex (c); 
   boost::add_edge (1, 2, c); 

   std::vector<int> component (boost::num_vertices (c));
   size_t num_components = boost::connected_components (c, &component[0]);
   ASSERT_EQ(num_components, 2);
}
