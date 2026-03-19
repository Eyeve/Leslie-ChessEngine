#include <gtest/gtest.h>

#include "tests.h"

namespace Leslie::Test {

TEST(pawn, test1) {
  PositionTest(
      "8/p5K1/1P5N/8/8/8/1k6/8 b - - 0 1",
      {{KING, SQ_B2, {SQ_A1, SQ_A2, SQ_A3, SQ_B1, SQ_B3, SQ_C1, SQ_C2, SQ_C3}},
       {PAWN, SQ_A7, {SQ_A6, SQ_B6, SQ_A5}}});
}

TEST(pawn, test2) {
  PositionTest("8/1k6/3pbn2/4P3/2p5/3P4/1PP5/K7 w - - 0 1",
               {{KING, SQ_A1, {SQ_A2, SQ_B1}},
                {
                    PAWN,
                    SQ_B2,
                    {SQ_B3, SQ_B4},
                },
                {
                    PAWN,
                    SQ_C2,
                    {SQ_C3},
                },
                {
                    PAWN,
                    SQ_D3,
                    {SQ_C4, SQ_D4},
                },
                {
                    PAWN,
                    SQ_E5,
                    {SQ_D6, SQ_F6},
                }});
}

TEST(pawn, test3) {
  PositionTest("1k6/3p1pp1/2Q2N2/2p5/N6p/B2K2P1/p7/7R b - - 0 1",
               {{KING, SQ_B8, {SQ_A7}},
                {
                    PAWN,
                    SQ_A2,
                    {SQ_A1},
                },
                {
                    PAWN,
                    SQ_C5,
                    {SQ_C4},
                },
                {
                    PAWN,
                    SQ_D7,
                    {SQ_D6, SQ_D5, SQ_C6},
                },
                {
                    PAWN,
                    SQ_G7,
                    {SQ_G6, SQ_G5, SQ_F6},
                },
                {
                    PAWN,
                    SQ_H4,
                    {SQ_G3, SQ_H3},
                }});
}

}  // namespace Leslie::test
