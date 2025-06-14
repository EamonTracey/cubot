#ifndef THISTLETHWAITE_SOLVER
#define THISTLETHWAITE_SOLVER

#include <cstdint>

#include "algorithm.h"
#include "cube.h"

namespace cubot {

class ThistlethwaiteSolver {
  public:
    static const int kThistlethwaitePruneTable1Size = 2048;
    static const int kThistlethwaitePruneTable2Size = 1082565;
    static const int kThistlethwaitePruneTable3Size = 352800;
    static const int kThistlethwaitePruneTable4Size = 663552;

    ThistlethwaiteSolver();
    ThistlethwaiteSolver(const std::string &path1, const std::string &path2,
                         const std::string &path3, const std::string &path4);
    ~ThistlethwaiteSolver() {}

    Algorithm Solve(const Cube &cube);

    static void
    GeneratePruneTable1(uint8_t prune_table_1[kThistlethwaitePruneTable1Size]);
    static void
    GeneratePruneTable2(uint8_t prune_table_2[kThistlethwaitePruneTable2Size]);
    static void
    GeneratePruneTable3(uint8_t prune_table_3[kThistlethwaitePruneTable3Size]);
    static void
    GeneratePruneTable4(uint8_t prune_table_4[kThistlethwaitePruneTable4Size]);

  private:
    uint8_t prune_table_1_[kThistlethwaitePruneTable1Size];
    uint8_t prune_table_2_[kThistlethwaitePruneTable2Size];
    uint8_t prune_table_3_[kThistlethwaitePruneTable3Size];
    uint8_t prune_table_4_[kThistlethwaitePruneTable4Size];
};

} // namespace cubot

#endif
