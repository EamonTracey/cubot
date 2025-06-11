#ifndef THISTLETHWAITE_SOLVER
#define THISTLETHWAITE_SOLVER

#include <cstdint>

#include "algorithm.h"
#include "cube.h"

namespace cubot {

class ThistlethwaiteSolver {
  public:
    ThistlethwaiteSolver();
    ~ThistlethwaiteSolver() {}

    Algorithm Solve(const Cube &cube);

    void GeneratePruneTable1(uint8_t prune_table_1[2048]);

  private:
    uint8_t prune_table_1_[2048];
};

} // namespace cubot

#endif
