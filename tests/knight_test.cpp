#include <gtest/gtest.h>

#include "tests.h"

namespace Leslie::Test {

TEST(knight, test1) {
  PositionTest("N7/8/8/8/6k1/8/1K6/8 w - - 0 1",
               {{KING, SQ_B2, {SQ_A1, SQ_A2, SQ_A3, SQ_B1, SQ_B3, SQ_C1, SQ_C2, SQ_C3}},
                {KNIGHT, SQ_A8, {SQ_C7, SQ_B6}}});
}

TEST(knight, test2) {
  PositionTest("8/K7/6n1/N7/5P2/8/1k6/8 b - - 0 1",
               {{KING, SQ_B2, {SQ_A1, SQ_A2, SQ_A3, SQ_B1, SQ_C1, SQ_C2, SQ_C3}},
                {KNIGHT, SQ_G6, {SQ_H8, SQ_F8, SQ_E7, SQ_E5, SQ_F4, SQ_H4}}});
}

}  // namespace Leslie::test
