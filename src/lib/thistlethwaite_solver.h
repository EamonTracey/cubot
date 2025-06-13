#ifndef THISTLETHWAITE_SOLVER
#define THISTLETHWAITE_SOLVER

#include <cstdint>

#include "algorithm.h"
#include "cube.h"

namespace cubot {

class ThistlethwaiteSolver {
  public:
    static const int kThistlethwaitePhase1Size = 2048;
    static const int kThistlethwaitePhase2Size = 1082565;
    static const int kThistlethwaitePhase3Size = 352800;
    static const int kThistlethwaitePhase4Size = 663552;

    ThistlethwaiteSolver();
    ~ThistlethwaiteSolver() {}

    Algorithm Solve(const Cube &cube);

    void GeneratePruneTable1(uint8_t prune_table_1[kThistlethwaitePhase1Size]);
    void GeneratePruneTable2(uint8_t prune_table_2[kThistlethwaitePhase2Size]);
    void GeneratePruneTable3(uint8_t prune_table_3[kThistlethwaitePhase3Size]);
    void GeneratePruneTable4(uint8_t prune_table_4[kThistlethwaitePhase4Size]);

  private:
    uint8_t prune_table_1_[kThistlethwaitePhase1Size];
    uint8_t prune_table_2_[kThistlethwaitePhase2Size];
    uint8_t prune_table_3_[kThistlethwaitePhase3Size];
    uint8_t prune_table_4_[kThistlethwaitePhase4Size];
};

} // namespace cubot

#endif
