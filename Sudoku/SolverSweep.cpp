#include "SolverSweep.h"
#include "SudokuDefines.h"
#include "FieldInfoStatic.h"
#include "SubSetPotentialsSweep.h"

#include<boost/container/static_vector.hpp>

using namespace Sudoku;


// !!!!!!!!!!!!!!!!!!!!!!!!!!!!

SolverSweepSubSet::SolverSweepSubSet(SubSetType type, ISubSetPotentialsSweep& sweep) : m_type(type), m_subSetSweep(sweep)
{}

SolverSweepResult SolverSweepSubSet::operator()(Potentials& potentials)
{
   bool anyChange = false;
   for (auto index : SubSetsAll)
   {
      auto subSetPotentials = potentials.getSubSetPotentials(m_type, index);
      if (m_subSetSweep(subSetPotentials))
      {
         anyChange = true;
      }
   }

   if (!anyChange) return SolverSweepResult::NoChange;
   if (potentials.isSolved()) return SolverSweepResult::Solved;
   return SolverSweepResult::Change;
}


// !!!!!!!!!!!!!!!!!!!!!!!!!!!!

SolverSweepClusters::SolverSweepClusters() :
   m_sweepCluster(),
   m_allTypes(m_sweepCluster)
{
}

SolverSweepResult SolverSweepClusters::operator()(Potentials& potentials)
{
   return m_allTypes(potentials);
}

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!


SolverSweepIterate::SolverSweepIterate(ISolverSweep& sweep) : m_sweep(sweep)
{}

SolverSweepResult SolverSweepIterate::operator()(Potentials& potentials)
{
   SolverSweepResult result = SolverSweepResult::NoChange;
   SolverSweepResult resultSweep = SolverSweepResult::NoChange;
   do
   {
      resultSweep = m_sweep(potentials);
      result = std::max(result, resultSweep);
   } while (resultSweep == SolverSweepResult::Change);
   return result;

};

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

SolverSweepComposite::SolverSweepComposite(std::initializer_list<ISolverSweep*> sweeps) :
   m_sweeps(sweeps)
{}

SolverSweepResult SolverSweepComposite::operator()(Potentials& potentials)
{
   SolverSweepResult result = SolverSweepResult::NoChange;
   for (auto* sweep : m_sweeps)
   {
      result = std::max(result, (*sweep)(potentials));
      if (result == SolverSweepResult::Solved) break;
   }
   return result;
}


// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

SolverSweepSubSetTypeAll::SolverSweepSubSetTypeAll(ISubSetPotentialsSweep& sweep) :
   m_row(SubSetType::Row, sweep),
   m_col(SubSetType::Column, sweep),
   m_subSquare(SubSetType::SubSquare, sweep),
   m_composite({ &m_row, &m_col, &m_subSquare })
{}


SolverSweepResult SolverSweepSubSetTypeAll::operator()(Potentials& potentials)
{
   return m_composite(potentials);
}

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!

SolverSweepTrivial::SolverSweepTrivial() :
   m_sweepSingles(),
   m_allTypes(m_sweepSingles),
   m_iterate(m_allTypes)
{
}

SolverSweepResult SolverSweepTrivial::operator()(Potentials& potentials)
{
   return m_iterate(potentials);
}

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

SolverSweep::SolverSweep() :
   m_sweepTrival(),
   m_sweepCluster(),
   m_composite({&m_sweepTrival, &m_sweepCluster}),
   m_iterate(m_composite)
{
}

SolverSweepResult SolverSweep::operator()(Potentials& potentials)
{
   return m_iterate(potentials);
}