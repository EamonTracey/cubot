#ifndef TWO_PHASE_SOLVER
#define TWO_PHASE_SOLVER

#include <cstdint>

#include "algorithm.h"
#include "cube.h"

namespace cubot {

class TwoPhaseSolver {
  public:
    static const int kTwoPhasePruneTable1Size = 1013760;
    static const int kTwoPhasePruneTable2Size = 1082565;
    static const int kTwoPhasePruneTable3Size = 2822400;
    static const int kTwoPhasePruneTable4Size = 2822400;

    TwoPhaseSolver();
    TwoPhaseSolver(const std::string &path1, const std::string &path2,
                   const std::string &path3, const std::string &path4);
    ~TwoPhaseSolver() {}

    Algorithm Solve(const Cube &cube);

    static void
    GeneratePruneTable1(uint8_t prune_table_1[kTwoPhasePruneTable1Size]);
    static void
    GeneratePruneTable2(uint8_t prune_table_2[kTwoPhasePruneTable2Size]);
    static void
    GeneratePruneTable3(uint8_t prune_table_3[kTwoPhasePruneTable3Size]);
    static void
    GeneratePruneTable4(uint8_t prune_table_4[kTwoPhasePruneTable4Size]);

  private:
    uint8_t prune_table_1_[kTwoPhasePruneTable1Size];
    uint8_t prune_table_2_[kTwoPhasePruneTable2Size];
    uint8_t prune_table_3_[kTwoPhasePruneTable3Size];
    uint8_t prune_table_4_[kTwoPhasePruneTable4Size];
};

} // namespace cubot

#endif
