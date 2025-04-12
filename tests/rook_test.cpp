#include <gtest/gtest.h>

#include "tests.h"

namespace Leslie::test {

TEST(rook, test1) {
  PositionTest(
      "8/5k2/8/1p1R1P2/8/1P6/8/K2b4 w - - 0 1",
      {
          {KING,
           SQ_A1,
           {SQ_A2, SQ_B2, SQ_B1}},
          {ROOK,
           SQ_D5,
           {SQ_D1, SQ_D2, SQ_D3, SQ_D4, SQ_D6,
            SQ_D7, SQ_D8, SQ_B5, SQ_C5, SQ_E5}},

          {PAWN, SQ_F5, {SQ_F6}},
          {PAWN, SQ_B3, {SQ_B4}},
      });
}

TEST(rook, test2) {
  PositionTest("k6K/8/8/8/8/6N1/1P4rN/6r1 b - - 0 1",
               {
                   {KING,
                    SQ_A8,
                    {SQ_A7, SQ_B7, SQ_B8}},
                   {ROOK,
                    SQ_G2,
                    {SQ_G3, SQ_H2, SQ_B2, SQ_C2,
                     SQ_D2, SQ_E2, SQ_F2}},
                   {ROOK,
                    SQ_G1,
                    {SQ_A1, SQ_B1, SQ_C1, SQ_D1,
                     SQ_E1, SQ_F1, SQ_H1}},
               });
}

}  // namespace leslie::test
