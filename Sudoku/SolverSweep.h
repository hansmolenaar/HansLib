#pragma once
#include "Potentials.h"
#include "SubSetPotentialsSweep.h"
#include "SudokuDefines.h"

#include <vector>

namespace Sudoku
{
enum class SolverSweepResult
{
    NoChange,
    Change,
    Solved
};

class ISolverSweep
{
  public:
    ~ISolverSweep() noexcept = default;
    virtual SolverSweepResult operator()(Potentials &potentials) = 0;
};

class SolverSweepIterate : public ISolverSweep
{
  public:
    explicit SolverSweepIterate(ISolverSweep &sweep);
    SolverSweepResult operator()(Potentials &potentials) override;

  private:
    ISolverSweep &m_sweep;
};

class SolverSweepComposite : public ISolverSweep
{
  public:
    explicit SolverSweepComposite(std::initializer_list<ISolverSweep *> sweep);
    SolverSweepResult operator()(Potentials &potentials) override;

  private:
    std::vector<ISolverSweep *> m_sweeps;
};

class SolverSweepSubSet : public ISolverSweep
{
  public:
    SolverSweepSubSet(RowColBoxType type, ISubSetPotentialsSweep &sweep);
    SolverSweepResult operator()(Potentials &potentials) override;

  private:
    RowColBoxType m_type;
    ISubSetPotentialsSweep &m_subSetSweep;
};

class SolverSweepSubSetTypeAll : public ISolverSweep
{
  public:
    SolverSweepSubSetTypeAll(ISubSetPotentialsSweep &sweep);
    SolverSweepResult operator()(Potentials &potentials) override;

  private:
    SolverSweepSubSet m_row;
    SolverSweepSubSet m_col;
    SolverSweepSubSet m_box;
    SolverSweepComposite m_composite;
};

class SolverSweepTrivial : public ISolverSweep
{
  public:
    SolverSweepTrivial();
    SolverSweepResult operator()(Potentials &potentials) override;

  private:
    SubSetPotentialsSweepSingles m_sweepSingles;
    SolverSweepSubSetTypeAll m_allTypes;
    SolverSweepIterate m_iterate;
};

class SolverSweepClusters : public ISolverSweep
{
  public:
    SolverSweepClusters();
    SolverSweepResult operator()(Potentials &potentials) override;

  private:
    SubSetPotentialsSweepClusters m_sweepCluster;
    SolverSweepSubSetTypeAll m_allTypes;
};

class SolverSweep : public ISolverSweep
{
  public:
    SolverSweep();
    SolverSweepResult operator()(Potentials &potentials) override;

  private:
    SolverSweepTrivial m_sweepTrival;
    SolverSweepClusters m_sweepCluster;
    SolverSweepComposite m_composite;
    SolverSweepIterate m_iterate;
};

} // namespace Sudoku