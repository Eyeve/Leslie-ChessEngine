#include <gtest/gtest.h>

#include "tests.h"

namespace Leslie::test {

TEST(bishop, test1) {
  PositionTest(
      "8/3k4/8/r5P1/5P2/8/3B1K2/2n5 w - - 0 1",
      {
          {KING, SQ_F2, {SQ_F1, SQ_G1, SQ_G2, SQ_G3, SQ_F3, SQ_E3, SQ_E1}},
          {BISHOP, SQ_D2, {SQ_C1, SQ_E1, SQ_C3, SQ_B4, SQ_A5, SQ_E3}},

          {PAWN, SQ_F4, {SQ_F5}},
          {PAWN, SQ_G5, {SQ_G6}},
      });
}

}  // namespace Leslie::test
