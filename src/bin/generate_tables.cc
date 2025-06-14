#include <cstdlib>

#include <fmt/core.h>

#include "prune.h"
#include "thistlethwaite_solver.h"
#include "two_phase_solver.h"

using namespace cubot;

int main(int argc, char *argv[]) {
    uint8_t *thistlethwaite1 = static_cast<uint8_t *>(
        malloc(sizeof(uint8_t) *
               ThistlethwaiteSolver::kThistlethwaitePruneTable1Size));
    ThistlethwaiteSolver::GeneratePruneTable1(thistlethwaite1);
    WritePruneTable("thistlethwaite1.pt", thistlethwaite1,
                    ThistlethwaiteSolver::kThistlethwaitePruneTable1Size);
    free(thistlethwaite1);
    fmt::print("✅ Thistlethwaite 1\n");

    uint8_t *thistlethwaite2 = static_cast<uint8_t *>(
        malloc(sizeof(uint8_t) *
               ThistlethwaiteSolver::kThistlethwaitePruneTable2Size));
    ThistlethwaiteSolver::GeneratePruneTable2(thistlethwaite2);
    WritePruneTable("thistlethwaite2.pt", thistlethwaite2,
                    ThistlethwaiteSolver::kThistlethwaitePruneTable2Size);
    free(thistlethwaite2);
    fmt::print("✅ Thistlethwaite 2\n");

    uint8_t *thistlethwaite3 = static_cast<uint8_t *>(
        malloc(sizeof(uint8_t) *
               ThistlethwaiteSolver::kThistlethwaitePruneTable3Size));
    ThistlethwaiteSolver::GeneratePruneTable3(thistlethwaite3);
    WritePruneTable("thistlethwaite3.pt", thistlethwaite3,
                    ThistlethwaiteSolver::kThistlethwaitePruneTable3Size);
    free(thistlethwaite3);
    fmt::print("✅ Thistlethwaite 3\n");

    uint8_t *thistlethwaite4 = static_cast<uint8_t *>(
        malloc(sizeof(uint8_t) *
               ThistlethwaiteSolver::kThistlethwaitePruneTable4Size));
    ThistlethwaiteSolver::GeneratePruneTable4(thistlethwaite4);
    WritePruneTable("thistlethwaite4.pt", thistlethwaite4,
                    ThistlethwaiteSolver::kThistlethwaitePruneTable4Size);
    free(thistlethwaite4);
    fmt::print("✅ Thistlethwaite 4\n");

    uint8_t *twophase1 = static_cast<uint8_t *>(
        malloc(sizeof(uint8_t) * TwoPhaseSolver::kTwoPhasePruneTable1Size));
    TwoPhaseSolver::GeneratePruneTable1(twophase1);
    WritePruneTable("twophase1.pt", twophase1,
                    TwoPhaseSolver::kTwoPhasePruneTable1Size);
    free(twophase1);
    fmt::print("✅ TwoPhase 1\n");

    uint8_t *twophase2 = static_cast<uint8_t *>(
        malloc(sizeof(uint8_t) * TwoPhaseSolver::kTwoPhasePruneTable2Size));
    TwoPhaseSolver::GeneratePruneTable2(twophase2);
    WritePruneTable("twophase2.pt", twophase2,
                    TwoPhaseSolver::kTwoPhasePruneTable2Size);
    free(twophase2);
    fmt::print("✅ TwoPhase 2\n");

    uint8_t *twophase3 = static_cast<uint8_t *>(
        malloc(sizeof(uint8_t) * TwoPhaseSolver::kTwoPhasePruneTable3Size));
    TwoPhaseSolver::GeneratePruneTable3(twophase3);
    WritePruneTable("twophase3.pt", twophase3,
                    TwoPhaseSolver::kTwoPhasePruneTable3Size);
    free(twophase3);
    fmt::print("✅ TwoPhase 3\n");

    uint8_t *twophase4 = static_cast<uint8_t *>(
        malloc(sizeof(uint8_t) * TwoPhaseSolver::kTwoPhasePruneTable4Size));
    TwoPhaseSolver::GeneratePruneTable4(twophase4);
    WritePruneTable("twophase4.pt", twophase4,
                    TwoPhaseSolver::kTwoPhasePruneTable4Size);
    free(twophase4);
    fmt::print("✅ TwoPhase 4\n");

    return EXIT_SUCCESS;
}
