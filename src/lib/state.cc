#include "state.h"

#include <utility>

static constexpr int kPowersOfTwo[] = {1,  2,   4,   8,   16,   32,
                                       64, 128, 256, 512, 1024, 2048};
static constexpr int kPowersOfThree[] = {1, 3, 9, 27, 81, 243, 729, 2187};

static constexpr int kNumberOfBits[2048] = {
    0, 1, 1, 2,  1, 2,  2,  3, 1, 2, 2, 3, 2, 3, 3, 4,  1, 2, 2, 3, 2, 3, 3, 4,
    2, 3, 3, 4,  3, 4,  4,  5, 1, 2, 2, 3, 2, 3, 3, 4,  2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4,  3, 4,  4,  5, 3, 4, 4, 5, 4, 5, 5, 6,  1, 2, 2, 3, 2, 3, 3, 4,
    2, 3, 3, 4,  3, 4,  4,  5, 2, 3, 3, 4, 3, 4, 4, 5,  3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4,  3, 4,  4,  5, 3, 4, 4, 5, 4, 5, 5, 6,  3, 4, 4, 5, 4, 5, 5, 6,
    4, 5, 5, 6,  5, 6,  6,  7, 1, 2, 2, 3, 2, 3, 3, 4,  2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4,  3, 4,  4,  5, 3, 4, 4, 5, 4, 5, 5, 6,  2, 3, 3, 4, 3, 4, 4, 5,
    3, 4, 4, 5,  4, 5,  5,  6, 3, 4, 4, 5, 4, 5, 5, 6,  4, 5, 5, 6, 5, 6, 6, 7,
    2, 3, 3, 4,  3, 4,  4,  5, 3, 4, 4, 5, 4, 5, 5, 6,  3, 4, 4, 5, 4, 5, 5, 6,
    4, 5, 5, 6,  5, 6,  6,  7, 3, 4, 4, 5, 4, 5, 5, 6,  4, 5, 5, 6, 5, 6, 6, 7,
    4, 5, 5, 6,  5, 6,  6,  7, 5, 6, 6, 7, 6, 7, 7, 8,  1, 2, 2, 3, 2, 3, 3, 4,
    2, 3, 3, 4,  3, 4,  4,  5, 2, 3, 3, 4, 3, 4, 4, 5,  3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4,  3, 4,  4,  5, 3, 4, 4, 5, 4, 5, 5, 6,  3, 4, 4, 5, 4, 5, 5, 6,
    4, 5, 5, 6,  5, 6,  6,  7, 2, 3, 3, 4, 3, 4, 4, 5,  3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5,  4, 5,  5,  6, 4, 5, 5, 6, 5, 6, 6, 7,  3, 4, 4, 5, 4, 5, 5, 6,
    4, 5, 5, 6,  5, 6,  6,  7, 4, 5, 5, 6, 5, 6, 6, 7,  5, 6, 6, 7, 6, 7, 7, 8,
    2, 3, 3, 4,  3, 4,  4,  5, 3, 4, 4, 5, 4, 5, 5, 6,  3, 4, 4, 5, 4, 5, 5, 6,
    4, 5, 5, 6,  5, 6,  6,  7, 3, 4, 4, 5, 4, 5, 5, 6,  4, 5, 5, 6, 5, 6, 6, 7,
    4, 5, 5, 6,  5, 6,  6,  7, 5, 6, 6, 7, 6, 7, 7, 8,  3, 4, 4, 5, 4, 5, 5, 6,
    4, 5, 5, 6,  5, 6,  6,  7, 4, 5, 5, 6, 5, 6, 6, 7,  5, 6, 6, 7, 6, 7, 7, 8,
    4, 5, 5, 6,  5, 6,  6,  7, 5, 6, 6, 7, 6, 7, 7, 8,  5, 6, 6, 7, 6, 7, 7, 8,
    6, 7, 7, 8,  7, 8,  8,  9, 1, 2, 2, 3, 2, 3, 3, 4,  2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4,  3, 4,  4,  5, 3, 4, 4, 5, 4, 5, 5, 6,  2, 3, 3, 4, 3, 4, 4, 5,
    3, 4, 4, 5,  4, 5,  5,  6, 3, 4, 4, 5, 4, 5, 5, 6,  4, 5, 5, 6, 5, 6, 6, 7,
    2, 3, 3, 4,  3, 4,  4,  5, 3, 4, 4, 5, 4, 5, 5, 6,  3, 4, 4, 5, 4, 5, 5, 6,
    4, 5, 5, 6,  5, 6,  6,  7, 3, 4, 4, 5, 4, 5, 5, 6,  4, 5, 5, 6, 5, 6, 6, 7,
    4, 5, 5, 6,  5, 6,  6,  7, 5, 6, 6, 7, 6, 7, 7, 8,  2, 3, 3, 4, 3, 4, 4, 5,
    3, 4, 4, 5,  4, 5,  5,  6, 3, 4, 4, 5, 4, 5, 5, 6,  4, 5, 5, 6, 5, 6, 6, 7,
    3, 4, 4, 5,  4, 5,  5,  6, 4, 5, 5, 6, 5, 6, 6, 7,  4, 5, 5, 6, 5, 6, 6, 7,
    5, 6, 6, 7,  6, 7,  7,  8, 3, 4, 4, 5, 4, 5, 5, 6,  4, 5, 5, 6, 5, 6, 6, 7,
    4, 5, 5, 6,  5, 6,  6,  7, 5, 6, 6, 7, 6, 7, 7, 8,  4, 5, 5, 6, 5, 6, 6, 7,
    5, 6, 6, 7,  6, 7,  7,  8, 5, 6, 6, 7, 6, 7, 7, 8,  6, 7, 7, 8, 7, 8, 8, 9,
    2, 3, 3, 4,  3, 4,  4,  5, 3, 4, 4, 5, 4, 5, 5, 6,  3, 4, 4, 5, 4, 5, 5, 6,
    4, 5, 5, 6,  5, 6,  6,  7, 3, 4, 4, 5, 4, 5, 5, 6,  4, 5, 5, 6, 5, 6, 6, 7,
    4, 5, 5, 6,  5, 6,  6,  7, 5, 6, 6, 7, 6, 7, 7, 8,  3, 4, 4, 5, 4, 5, 5, 6,
    4, 5, 5, 6,  5, 6,  6,  7, 4, 5, 5, 6, 5, 6, 6, 7,  5, 6, 6, 7, 6, 7, 7, 8,
    4, 5, 5, 6,  5, 6,  6,  7, 5, 6, 6, 7, 6, 7, 7, 8,  5, 6, 6, 7, 6, 7, 7, 8,
    6, 7, 7, 8,  7, 8,  8,  9, 3, 4, 4, 5, 4, 5, 5, 6,  4, 5, 5, 6, 5, 6, 6, 7,
    4, 5, 5, 6,  5, 6,  6,  7, 5, 6, 6, 7, 6, 7, 7, 8,  4, 5, 5, 6, 5, 6, 6, 7,
    5, 6, 6, 7,  6, 7,  7,  8, 5, 6, 6, 7, 6, 7, 7, 8,  6, 7, 7, 8, 7, 8, 8, 9,
    4, 5, 5, 6,  5, 6,  6,  7, 5, 6, 6, 7, 6, 7, 7, 8,  5, 6, 6, 7, 6, 7, 7, 8,
    6, 7, 7, 8,  7, 8,  8,  9, 5, 6, 6, 7, 6, 7, 7, 8,  6, 7, 7, 8, 7, 8, 8, 9,
    6, 7, 7, 8,  7, 8,  8,  9, 7, 8, 8, 9, 8, 9, 9, 10, 1, 2, 2, 3, 2, 3, 3, 4,
    2, 3, 3, 4,  3, 4,  4,  5, 2, 3, 3, 4, 3, 4, 4, 5,  3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4,  3, 4,  4,  5, 3, 4, 4, 5, 4, 5, 5, 6,  3, 4, 4, 5, 4, 5, 5, 6,
    4, 5, 5, 6,  5, 6,  6,  7, 2, 3, 3, 4, 3, 4, 4, 5,  3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5,  4, 5,  5,  6, 4, 5, 5, 6, 5, 6, 6, 7,  3, 4, 4, 5, 4, 5, 5, 6,
    4, 5, 5, 6,  5, 6,  6,  7, 4, 5, 5, 6, 5, 6, 6, 7,  5, 6, 6, 7, 6, 7, 7, 8,
    2, 3, 3, 4,  3, 4,  4,  5, 3, 4, 4, 5, 4, 5, 5, 6,  3, 4, 4, 5, 4, 5, 5, 6,
    4, 5, 5, 6,  5, 6,  6,  7, 3, 4, 4, 5, 4, 5, 5, 6,  4, 5, 5, 6, 5, 6, 6, 7,
    4, 5, 5, 6,  5, 6,  6,  7, 5, 6, 6, 7, 6, 7, 7, 8,  3, 4, 4, 5, 4, 5, 5, 6,
    4, 5, 5, 6,  5, 6,  6,  7, 4, 5, 5, 6, 5, 6, 6, 7,  5, 6, 6, 7, 6, 7, 7, 8,
    4, 5, 5, 6,  5, 6,  6,  7, 5, 6, 6, 7, 6, 7, 7, 8,  5, 6, 6, 7, 6, 7, 7, 8,
    6, 7, 7, 8,  7, 8,  8,  9, 2, 3, 3, 4, 3, 4, 4, 5,  3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5,  4, 5,  5,  6, 4, 5, 5, 6, 5, 6, 6, 7,  3, 4, 4, 5, 4, 5, 5, 6,
    4, 5, 5, 6,  5, 6,  6,  7, 4, 5, 5, 6, 5, 6, 6, 7,  5, 6, 6, 7, 6, 7, 7, 8,
    3, 4, 4, 5,  4, 5,  5,  6, 4, 5, 5, 6, 5, 6, 6, 7,  4, 5, 5, 6, 5, 6, 6, 7,
    5, 6, 6, 7,  6, 7,  7,  8, 4, 5, 5, 6, 5, 6, 6, 7,  5, 6, 6, 7, 6, 7, 7, 8,
    5, 6, 6, 7,  6, 7,  7,  8, 6, 7, 7, 8, 7, 8, 8, 9,  3, 4, 4, 5, 4, 5, 5, 6,
    4, 5, 5, 6,  5, 6,  6,  7, 4, 5, 5, 6, 5, 6, 6, 7,  5, 6, 6, 7, 6, 7, 7, 8,
    4, 5, 5, 6,  5, 6,  6,  7, 5, 6, 6, 7, 6, 7, 7, 8,  5, 6, 6, 7, 6, 7, 7, 8,
    6, 7, 7, 8,  7, 8,  8,  9, 4, 5, 5, 6, 5, 6, 6, 7,  5, 6, 6, 7, 6, 7, 7, 8,
    5, 6, 6, 7,  6, 7,  7,  8, 6, 7, 7, 8, 7, 8, 8, 9,  5, 6, 6, 7, 6, 7, 7, 8,
    6, 7, 7, 8,  7, 8,  8,  9, 6, 7, 7, 8, 7, 8, 8, 9,  7, 8, 8, 9, 8, 9, 9, 10,
    2, 3, 3, 4,  3, 4,  4,  5, 3, 4, 4, 5, 4, 5, 5, 6,  3, 4, 4, 5, 4, 5, 5, 6,
    4, 5, 5, 6,  5, 6,  6,  7, 3, 4, 4, 5, 4, 5, 5, 6,  4, 5, 5, 6, 5, 6, 6, 7,
    4, 5, 5, 6,  5, 6,  6,  7, 5, 6, 6, 7, 6, 7, 7, 8,  3, 4, 4, 5, 4, 5, 5, 6,
    4, 5, 5, 6,  5, 6,  6,  7, 4, 5, 5, 6, 5, 6, 6, 7,  5, 6, 6, 7, 6, 7, 7, 8,
    4, 5, 5, 6,  5, 6,  6,  7, 5, 6, 6, 7, 6, 7, 7, 8,  5, 6, 6, 7, 6, 7, 7, 8,
    6, 7, 7, 8,  7, 8,  8,  9, 3, 4, 4, 5, 4, 5, 5, 6,  4, 5, 5, 6, 5, 6, 6, 7,
    4, 5, 5, 6,  5, 6,  6,  7, 5, 6, 6, 7, 6, 7, 7, 8,  4, 5, 5, 6, 5, 6, 6, 7,
    5, 6, 6, 7,  6, 7,  7,  8, 5, 6, 6, 7, 6, 7, 7, 8,  6, 7, 7, 8, 7, 8, 8, 9,
    4, 5, 5, 6,  5, 6,  6,  7, 5, 6, 6, 7, 6, 7, 7, 8,  5, 6, 6, 7, 6, 7, 7, 8,
    6, 7, 7, 8,  7, 8,  8,  9, 5, 6, 6, 7, 6, 7, 7, 8,  6, 7, 7, 8, 7, 8, 8, 9,
    6, 7, 7, 8,  7, 8,  8,  9, 7, 8, 8, 9, 8, 9, 9, 10, 3, 4, 4, 5, 4, 5, 5, 6,
    4, 5, 5, 6,  5, 6,  6,  7, 4, 5, 5, 6, 5, 6, 6, 7,  5, 6, 6, 7, 6, 7, 7, 8,
    4, 5, 5, 6,  5, 6,  6,  7, 5, 6, 6, 7, 6, 7, 7, 8,  5, 6, 6, 7, 6, 7, 7, 8,
    6, 7, 7, 8,  7, 8,  8,  9, 4, 5, 5, 6, 5, 6, 6, 7,  5, 6, 6, 7, 6, 7, 7, 8,
    5, 6, 6, 7,  6, 7,  7,  8, 6, 7, 7, 8, 7, 8, 8, 9,  5, 6, 6, 7, 6, 7, 7, 8,
    6, 7, 7, 8,  7, 8,  8,  9, 6, 7, 7, 8, 7, 8, 8, 9,  7, 8, 8, 9, 8, 9, 9, 10,
    4, 5, 5, 6,  5, 6,  6,  7, 5, 6, 6, 7, 6, 7, 7, 8,  5, 6, 6, 7, 6, 7, 7, 8,
    6, 7, 7, 8,  7, 8,  8,  9, 5, 6, 6, 7, 6, 7, 7, 8,  6, 7, 7, 8, 7, 8, 8, 9,
    6, 7, 7, 8,  7, 8,  8,  9, 7, 8, 8, 9, 8, 9, 9, 10, 5, 6, 6, 7, 6, 7, 7, 8,
    6, 7, 7, 8,  7, 8,  8,  9, 6, 7, 7, 8, 7, 8, 8, 9,  7, 8, 8, 9, 8, 9, 9, 10,
    6, 7, 7, 8,  7, 8,  8,  9, 7, 8, 8, 9, 8, 9, 9, 10, 7, 8, 8, 9, 8, 9, 9, 10,
    8, 9, 9, 10, 9, 10, 10, 11};

static constexpr int kNPermuteR[12][12] = {{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 3, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 4, 12, 24, 24, 0, 0, 0, 0, 0, 0, 0}, {1, 5, 20, 60, 120, 120, 0, 0, 0, 0, 0, 0}, {1, 6, 30, 120, 360, 720, 720, 0, 0, 0, 0, 0}, {1, 7, 42, 210, 840, 2520, 5040, 5040, 0, 0, 0, 0}, {1, 8, 56, 336, 1680, 6720, 20160, 40320, 40320, 0, 0, 0}, {1, 9, 72, 504, 3024, 15120, 60480, 181440, 362880, 362880, 0, 0}, {1, 10, 90, 720, 5040, 30240, 151200, 604800, 1814400, 3628800, 3628800, 0}, {1, 11, 110, 990, 7920, 55440, 332640, 1663200, 6652800, 19958400, 39916800, 39916800}};

static constexpr int kNChooseR[12][12] = {{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 3, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 4, 6, 4, 1, 0, 0, 0, 0, 0, 0, 0}, {1, 5, 10, 10, 5, 1, 0, 0, 0, 0, 0, 0}, {1, 6, 15, 20, 15, 6, 1, 0, 0, 0, 0, 0}, {1, 7, 21, 35, 35, 21, 7, 1, 0, 0, 0, 0}, {1, 8, 28, 56, 70, 56, 28, 8, 1, 0, 0, 0}, {1, 9, 36, 84, 126, 126, 84, 36, 9, 1, 0, 0}, {1, 10, 45, 120, 210, 252, 210, 120, 45, 10, 1, 0}, {1, 11, 55, 165, 330, 462, 462, 330, 165, 55, 11, 1}};

namespace cubot {

int CalculatePermutationState(const std::vector<int> &permutation, size_t n) {
    size_t k = permutation.size();

    int visited = 0;
    int state = 0;
    for (size_t i = 0; i < k; ++i) {
        int number = permutation[i];
        int lehmer = number - kNumberOfBits[visited & ((1 << number) - 1)];
        state += lehmer * kNPermuteR[n - i - 1][k - i - 1];
        visited |= 1 << number;
    }

    return state;
}

int CalculateCombinationState(const std::vector<int> &combination) {
    int state = 0;
    for (size_t i = 0; i < combination.size(); ++i) {
        int number = combination[i];
        state += kNChooseR[number][i + 1];
    }

    return state;
}

template <typename Iterable, typename Predicate>
int CalculateCombinationState(const Iterable &elements, Predicate predicate) {
    static_assert(
        std::is_same_v<
            decltype(predicate(
                std::declval<std::decay_t<decltype(*begin(elements))>>())),
            bool>,
        "Predicate must be callable with element type and return bool.");

    int state = 0;
    int i = 0;
    int r = 1;
    for (auto &element : elements) {
        if (predicate(element)) {
            state += kNChooseR[i][r];
            ++r;
        }
        ++i;
    }

    return state;
}

int CalculateEdgeOrientationState(const Cube &cube) {
    auto &edges = cube.edges();

    int state = 0;
    for (size_t i = 0; i < 11; ++i)
        state += static_cast<int>(edges[i].orientation) * kPowersOfTwo[i];

    return state;
}

int CalculateCornerOrientationState(const Cube &cube) {
    auto &corners = cube.corners();

    int state = 0;
    for (size_t i = 0; i < 7; ++i)
        state += static_cast<int>(corners[i].orientation) * kPowersOfThree[i];

    return state;
}

int CalculateEquatorialEdgeCombinationState(const Cube &cube) {
    auto &edges = cube.edges();

    int state = CalculateCombinationState(edges, [](const Cube::Edge &edge) {
        return edge.solvedPosition == Cube::Edge::Position::kRightFront ||
               edge.solvedPosition == Cube::Edge::Position::kRightBack ||
               edge.solvedPosition == Cube::Edge::Position::kLeftFront ||
               edge.solvedPosition == Cube::Edge::Position::kLeftBack;
    });

    return state;
}

int CalculateCornerOrientationEquatorialEdgeCombinationState(const Cube &cube) {
    int corner_orientation_state = CalculateCornerOrientationState(cube);
    int equatorial_edge_combination_state =
        CalculateEquatorialEdgeCombinationState(cube);

    int state =
        corner_orientation_state + equatorial_edge_combination_state * 2187;

    return state;
}

// Given A, compute its inverse permutation
std::vector<int> invert_permutation(const std::vector<int>& A) {
    std::vector<int> P(A.size());
    for (int i = 0; i < A.size(); ++i)
        P[A[i]] = i;
    return P;
}

std::vector<int> apply_permutation(const std::vector<int>& B, const std::vector<int>& P) {
    std::vector<int> C(P.size());
    for (int i = 0; i < P.size(); ++i)
        C[i] = B[P[i]];
    return C;
}

int CalculateTetradTwistState(const Cube &cube) {
//    // URF=0 ULB=1 DRB=2 DLF=3
//    // URB=0 ULF=1 DRF=2 DLB=3
    int QQQ[] = {0, 0, 1, 1, 2, 2, 3, 3};

    std::vector<int> tetrad1;
    std::vector<int> tetrad2;
    for (auto &corner : cube.corners()) {
        int position = static_cast<int>(corner.solvedPosition);
        if (0b01101001 & (1 << position))
            tetrad1.push_back(QQQ[position]);
        else
            tetrad2.push_back(QQQ[position]);
    }

    auto swap = [&](int a, int b, int c, int d) {
        std::swap(tetrad1[a], tetrad1[b]);
        std::swap(tetrad2[c], tetrad2[d]);
    };

    // URF=0 ULB=1 DRB=2 DLF=3
    // URB=0 ULF=1 DRF=2 DLB=3

    // URF
    if (tetrad1[0] == 1) swap(0, 1, 0, 1); // U2
    else if (tetrad1[0] == 2) swap(0, 2, 0, 2); // R2
    else if (tetrad1[0] == 3) swap(0, 3, 1, 2); // F2

    // ULB
    if (tetrad1[1] == 2) swap(1, 2, 0, 3); // B2
    else if (tetrad1[1] == 3) swap(1, 3, 1, 3); // L2

    // DRB, DLF
    if (tetrad1[2] == 3) swap(2, 3, 2, 3); // D2

//    std::cout << "t1:";
//    for (auto t1:tetrad1)
//        std::cout << " " << t1;
//    std::cout << std::endl << "t2:";
//    for (auto t2:tetrad2)
//        std::cout << " " << t2;
//    std::cout << std::endl;

    // solve URB
//    if (tetrad2[0] == 1) { swap(0, 1, 0, 1); swap(1, 3, 1, 3); swap(0, 3, 1, 2); swap(1, 3, 1, 3); } // U2 L2 F2 L2
//    else if (tetrad2[0] == 2) { swap(0, 2, 0, 2); swap(2, 3, 2, 3); swap(0, 3, 1, 2); swap(2, 3, 2, 3); } // R2 D2 F2 D2
//    else if (tetrad2[0] == 3) { swap(1, 2, 0, 3); swap(0, 2, 0, 2); swap(0, 1, 0, 1); swap(0, 2, 0, 2);  } // B2 R2 U2 R2

//    tetrad1 = apply_permutation(tetrad1, invert_permutation(tetrad1));
//    tetrad2 = apply_permutation(tetrad2, invert_permutation(tetrad1));

    //find
    int q;
    for (int i = 0; i < 4; ++i){
        if (tetrad2[i]==0) q=i;
    }
//    if (q==1) { std::swap(tetrad2[0], tetrad2[1]); std::swap(tetrad2[2], tetrad2[3]);  }
//    if (q==2) { std::swap(tetrad2[0], tetrad2[2]); std::swap(tetrad2[1], tetrad2[3]);  }
//    if (q==3) { std::swap(tetrad2[0], tetrad2[3]); std::swap(tetrad2[1], tetrad2[2]);  }
    if (q==1) { swap(0, 1, 0, 1); swap(1, 3, 1, 3); swap(0, 3, 1, 2); swap(1, 3, 1, 3); } // U2 L2 F2 L2
    else if (q == 2) { swap(0, 2, 0, 2); swap(2, 3, 2, 3); swap(0, 3, 1, 2); swap(2, 3, 2, 3); } // R2 D2 F2 D2
    else if (q == 3) { swap(1, 2, 0, 3); swap(0, 2, 0, 2); swap(0, 1, 0, 1); swap(0, 2, 0, 2);  } // B2 R2 U2 R2

    if (tetrad2[1] == 1 && tetrad2[2] == 2) return 0;
    if (tetrad2[1] == 1 && tetrad2[2] == 3) return 1;
    if (tetrad2[1] == 2 && tetrad2[2] == 1) return 2;
    if (tetrad2[1] == 2 && tetrad2[2] == 3) return 3;
    if (tetrad2[1] == 3 && tetrad2[2] == 1) return 4;
    if (tetrad2[1] == 3 && tetrad2[2] == 2) return 5;

    throw std::runtime_error("NOOOOOOOOO");
    return 3;
}

int CalculateThistlethwaiteMiddleEdgeCombinationFirstTetradCombinationTetradTwistState(
    const Cube &cube) {
    auto &edges = cube.edges();
    auto &corners = cube.corners();

    // This function assumes the equatorial edges are in the equatorial slice.
    // Conveniently, the set indices that correspond to the equatorial edges is
    // {8, 9, 10, 11}. Therefore, the index of each middle edge is in the set
    // {0, 1, 2, 3, 4, 5, 6, 7}, so we do not need to shrink the elements input
    // for this 4-combination of 8 elements. We could choose to pass
    // std::vector(edges.begin(), edges.begin() + 8).
    int middle_edge_combination_state =
        CalculateCombinationState(edges, [](const Cube::Edge &edge) {
            return edge.solvedPosition == Cube::Edge::Position::kUpFront ||
                   edge.solvedPosition == Cube::Edge::Position::kUpBack ||
                   edge.solvedPosition == Cube::Edge::Position::kDownFront ||
                   edge.solvedPosition == Cube::Edge::Position::kDownBack;
        });
    int first_tetrad_combination_state =
        CalculateCombinationState(corners, [](const Cube::Corner &corner) {
            return corner.solvedPosition ==
                       Cube::Corner::Position::kUpRightFront ||
                   corner.solvedPosition ==
                       Cube::Corner::Position::kUpLeftBack ||
                   corner.solvedPosition ==
                       Cube::Corner::Position::kDownRightBack ||
                   corner.solvedPosition ==
                       Cube::Corner::Position::kDownLeftFront;
        });

    int tetrad_twist_state = CalculateTetradTwistState(cube);

    int state =
        middle_edge_combination_state +
        first_tetrad_combination_state * 70 + tetrad_twist_state * 4900;

    return state;
}

int CalculateCrossState(const Cube &cube) {
    auto &edges = cube.edges();

    int orientation[4];
    int permutation[4];
    for (size_t i = 0; i < 12; ++i) {
        auto &edge = edges[i];
        if (static_cast<int>(edge.solvedPosition) < 4) {
            orientation[static_cast<int>(edge.solvedPosition)] =
                static_cast<int>(edge.orientation);
            permutation[static_cast<int>(edge.solvedPosition)] =
                static_cast<int>(i);
        }
    }

    int orientation_state = 0;
    for (size_t i = 0; i < 4; ++i)
        orientation_state += orientation[i] * kPowersOfTwo[i];
    int permutation_state = CalculatePermutationState(
        std::vector<int>(permutation, permutation + 4), 12);
    int state = orientation_state + permutation_state * 16;

    return state;
}

} // namespace cubot
