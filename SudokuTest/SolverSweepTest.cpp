#include <gtest/gtest.h>

#include "Diagram.h"
#include "FieldInfoStatic.h"
#include "SolverSweep.h"
#include "TestModels.h"

#include "boost/graph/adjacency_list.hpp"
#include <boost/graph/connected_components.hpp>
#include <boost/graph/sequential_vertex_coloring.hpp>
#include <iostream>
#include <unordered_map>

using namespace Sudoku;
using namespace boost;

TEST(SolverSweepTest, SolverSweepAll)
{
    SolverSweep sweep;
    Diagram diagramEmpty = TestModels::getEmpty();
    auto potentials = diagramEmpty.getPotentials();
    auto result = sweep(potentials);
    ASSERT_EQ(result, SolverSweepResult::NoChange);
}

TEST(SolverSweepTest, SolverSweepSolved)
{
    SolverSweep sweep;
    Diagram diagramSolved = TestModels::getSolved();
    ASSERT_TRUE(diagramSolved.isSolved());
    auto potentials = diagramSolved.getPotentials();
    auto result = sweep(potentials);
    ASSERT_EQ(result, SolverSweepResult::Solved);
}
