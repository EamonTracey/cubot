#include <cstdlib>

#include "prune.h"
#include "thistlethwaite_solver.h"

using namespace cubot;

int main(int argc, char *argv[]) {
    uint8_t *thistlethwaite1 = reinterpret_cast<uint8_t *>(malloc(
        sizeof(uint8_t) * ThistlethwaiteSolver::kThistlethwaitePhase1Size));
    ThistlethwaiteSolver::GeneratePruneTable1(thistlethwaite1);
    WritePruneTable("thistlethwaite1.pt", thistlethwaite1,
                    ThistlethwaiteSolver::kThistlethwaitePhase1Size);
    free(thistlethwaite1);

    uint8_t *thistlethwaite2 = reinterpret_cast<uint8_t *>(malloc(
        sizeof(uint8_t) * ThistlethwaiteSolver::kThistlethwaitePhase2Size));
    ThistlethwaiteSolver::GeneratePruneTable2(thistlethwaite2);
    WritePruneTable("thistlethwaite2.pt", thistlethwaite2,
                    ThistlethwaiteSolver::kThistlethwaitePhase2Size);
    free(thistlethwaite2);

    uint8_t *thistlethwaite3 = reinterpret_cast<uint8_t *>(malloc(
        sizeof(uint8_t) * ThistlethwaiteSolver::kThistlethwaitePhase3Size));
    ThistlethwaiteSolver::GeneratePruneTable3(thistlethwaite3);
    WritePruneTable("thistlethwaite3.pt", thistlethwaite3,
                    ThistlethwaiteSolver::kThistlethwaitePhase3Size);
    free(thistlethwaite3);

    uint8_t *thistlethwaite4 = reinterpret_cast<uint8_t *>(malloc(
        sizeof(uint8_t) * ThistlethwaiteSolver::kThistlethwaitePhase4Size));
    ThistlethwaiteSolver::GeneratePruneTable4(thistlethwaite4);
    WritePruneTable("thistlethwaite4.pt", thistlethwaite4,
                    ThistlethwaiteSolver::kThistlethwaitePhase4Size);
    free(thistlethwaite4);

    return EXIT_SUCCESS;
}
