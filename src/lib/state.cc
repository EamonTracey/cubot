#include "state.h"

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

static constexpr int kNPermuteR[12][12] = {
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 3, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 4, 12, 24, 24, 0, 0, 0, 0, 0, 0, 0},
    {1, 5, 20, 60, 120, 120, 0, 0, 0, 0, 0, 0},
    {1, 6, 30, 120, 360, 720, 720, 0, 0, 0, 0, 0},
    {1, 7, 42, 210, 840, 2520, 5040, 5040, 0, 0, 0, 0},
    {1, 8, 56, 336, 1680, 6720, 20160, 40320, 40320, 0, 0, 0},
    {1, 9, 72, 504, 3024, 15120, 60480, 181440, 362880, 362880, 0, 0},
    {1, 10, 90, 720, 5040, 30240, 151200, 604800, 1814400, 3628800, 3628800, 0},
    {1, 11, 110, 990, 7920, 55440, 332640, 1663200, 6652800, 19958400, 39916800,
     39916800}};

static constexpr int kNChooseR[12][12] = {
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 3, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 4, 6, 4, 1, 0, 0, 0, 0, 0, 0, 0},
    {1, 5, 10, 10, 5, 1, 0, 0, 0, 0, 0, 0},
    {1, 6, 15, 20, 15, 6, 1, 0, 0, 0, 0, 0},
    {1, 7, 21, 35, 35, 21, 7, 1, 0, 0, 0, 0},
    {1, 8, 28, 56, 70, 56, 28, 8, 1, 0, 0, 0},
    {1, 9, 36, 84, 126, 126, 84, 36, 9, 1, 0, 0},
    {1, 10, 45, 120, 210, 252, 210, 120, 45, 10, 1, 0},
    {1, 11, 55, 165, 330, 462, 462, 330, 165, 55, 11, 1}};

static constexpr int kStanding = 0;
static constexpr int kMiddle = 1;
static constexpr int kEquatorial = 2;
static constexpr int kSlice[12] = {0, 0, 1, 1, 0, 0, 1, 1, 2, 2, 2, 2};
static constexpr int kStandingEdges[4] = {0, 1, 4, 5};
static constexpr int kMiddleEdges[4] = {2, 3, 6, 7};
static constexpr int kEquatorialEdges[4] = {8, 9, 10, 11};
static constexpr int kNormalizeStandingEdge[12] = {0, 1,  -1, -1, 2,
                                                   3, -1, -1, -1, -1};
static constexpr int kNormalizeMiddleEdge[12] = {-1, -1, 0,  1,  -1, -1,
                                                 2,  3,  -1, -1, -1, -1};
static constexpr int kNormalizeEquatorialEdge[12] = {-1, -1, -1, -1, -1, -1,
                                                     -1, -1, 0,  1,  2,  3};

static constexpr int kOuter = 0;
static constexpr int kInner = 1;
static constexpr int kTetrad[8] = {0, 1, 1, 0, 1, 0, 0, 1};
static constexpr int kOuterCorners[4] = {0, 3, 5, 6};
static constexpr int kInnerCorners[4] = {1, 2, 4, 7};
static constexpr int kNormalizeOuterCorner[8] = {0, -1, -1, 1, -1, 2, 3, -1};
static constexpr int kNormalizeInnerCorner[8] = {-1, 0, 1, -1, 2, -1, -1, 3};

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

int CalculateEdgePermutationState(const Cube &cube) {
    std::vector<int> permutation;
    for (auto edge : cube.edges())
        permutation.push_back(static_cast<int>(edge.solvedPosition));
    return CalculatePermutationState(permutation, 12);
}

int CalculateCornerPermutationState(const Cube &cube) {
    std::vector<int> permutation;
    for (auto corner : cube.corners())
        permutation.push_back(static_cast<int>(corner.solvedPosition));
    return CalculatePermutationState(permutation, 8);
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

int CalculateParityState(const Cube &cube) {
    auto &corners = cube.corners();

    int inversions = 0;
    for (size_t i = 0; i < 8; ++i)
        for (size_t j = i + 1; j < 8; ++j)
            if (static_cast<int>(corners[i].solvedPosition) <
                static_cast<int>(corners[j].solvedPosition))
                inversions++;

    int state = inversions % 2;

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

int CalculateG2MiddleEdgeCombinationTetradPairsCombinationState(
    const Cube &cube) {
    auto &edges = cube.edges();
    auto &corners = cube.corners();

    // TODO: Explain better.
    const std::array<Cube::Corner, 8> &corners8 = corners;
    std::array<Cube::Corner, 6> corners6;
    std::array<Cube::Corner, 4> corners4;
    size_t i6 = 0;
    size_t i4 = 0;
    for (const auto &corner : corners) {
        if (corner.solvedPosition != Cube::Corner::Position::kUpRightFront &&
            corner.solvedPosition != Cube::Corner::Position::kUpLeftBack) {
            corners6[i6++] = corner;
            if (corner.solvedPosition != Cube::Corner::Position::kUpRightBack &&
                corner.solvedPosition != Cube::Corner::Position::kUpLeftFront)
                corners4[i4++] = corner;
        }
    }

    // This function assumes the equatorial edges are in the equatorial slice.
    // Conveniently, the set indices that correspond to the equatorial edges is
    // {8, 9, 10, 11}. Therefore, the index of each middle edge is in the set
    // {0, 1, 2, 3, 4, 5, 6, 7}, so we do not need to shrink the elements input
    // for this 4-combination of 8 elements. We could choose to pass
    // std::vector(edges.begin(), edges.begin() + 8).
    int g2_middle_edge_combination_state =
        CalculateCombinationState(edges, [](const Cube::Edge &edge) {
            return edge.solvedPosition == Cube::Edge::Position::kUpFront ||
                   edge.solvedPosition == Cube::Edge::Position::kUpBack ||
                   edge.solvedPosition == Cube::Edge::Position::kDownFront ||
                   edge.solvedPosition == Cube::Edge::Position::kDownBack;
        });
    int pair8_combination_state =
        CalculateCombinationState(corners8, [](const Cube::Corner &corner) {
            return corner.solvedPosition ==
                       Cube::Corner::Position::kUpRightFront ||
                   corner.solvedPosition == Cube::Corner::Position::kUpLeftBack;
        });

    int pair6_combination_state =
        CalculateCombinationState(corners6, [](const Cube::Corner &corner) {
            return corner.solvedPosition ==
                       Cube::Corner::Position::kUpRightBack ||
                   corner.solvedPosition ==
                       Cube::Corner::Position::kUpLeftFront;
        });
    int pair4_combination_state =
        CalculateCombinationState(corners4, [](const Cube::Corner &corner) {
            return corner.solvedPosition ==
                       Cube::Corner::Position::kDownRightFront ||
                   corner.solvedPosition ==
                       Cube::Corner::Position::kDownLeftBack;
        });
    int parity_state = CalculateParityState(cube);

    int state = g2_middle_edge_combination_state +
                pair8_combination_state * 70 + pair6_combination_state * 1960 +
                pair4_combination_state * 29400 + parity_state * 176400;

    return state;
}

int CalculateG3State(const Cube &cube) {
    auto &edges = cube.edges();
    auto &corners = cube.corners();

    // TODO: Explain better.
    std::vector<int> standing_permutation;
    std::vector<int> middle_permutation;
    std::vector<int> equatorial_permutation;
    std::vector<int> outer_permutation;
    std::vector<int> inner_permutation;
    for (int i : kStandingEdges)
        standing_permutation.push_back(kNormalizeStandingEdge[static_cast<int>(
            edges[static_cast<size_t>(i)].solvedPosition)]);
    for (int i : kMiddleEdges)
        middle_permutation.push_back(kNormalizeMiddleEdge[static_cast<int>(
            edges[static_cast<size_t>(i)].solvedPosition)]);
    for (int i : kEquatorialEdges)
        equatorial_permutation.push_back(
            kNormalizeEquatorialEdge[static_cast<int>(
                edges[static_cast<size_t>(i)].solvedPosition)]);
    for (int i : kOuterCorners)
        outer_permutation.push_back(kNormalizeOuterCorner[static_cast<int>(
            corners[static_cast<size_t>(i)].solvedPosition)]);
    for (int i : kInnerCorners)
        inner_permutation.push_back(kNormalizeInnerCorner[static_cast<int>(
            corners[static_cast<size_t>(i)].solvedPosition)]);

    int standing_permutation_state =
        CalculatePermutationState(standing_permutation, 4);
    int middle_permutation_state =
        CalculatePermutationState(middle_permutation, 4);
    int equatorial_permutation_state = CalculatePermutationState(
        std::vector(equatorial_permutation.begin(),
                    equatorial_permutation.begin() + 2),
        4);
    int outer_permutation_state =
        CalculatePermutationState(outer_permutation, 4);
    int inner_permutation_state = CalculatePermutationState(
        std::vector(inner_permutation.begin(), inner_permutation.begin() + 1),
        4);

    int state = standing_permutation_state + middle_permutation_state * 24 +
                equatorial_permutation_state * 576 +
                outer_permutation_state * 6912 +
                inner_permutation_state * 165888;

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
