#include <gtest/gtest.h>

#include "tests.h"

namespace Leslie::test {
TEST(advanced, test1) {
  PositionTest("8/nK6/2Nrpq1P/4P3/3P4/2r3k1/1P4N1/8 w - - 0 1",
               {
                   {KING, SQ_B7, {SQ_B6, SQ_A6, SQ_A7, SQ_A8, SQ_B8, SQ_C7}},

                   {KNIGHT, SQ_C6, {SQ_B4, SQ_A5, SQ_A7, SQ_B8, SQ_D8, SQ_E7}},

                   {KNIGHT, SQ_G2, {SQ_E1, SQ_E3, SQ_F4, SQ_H4}},

                   {PAWN, SQ_B2, {SQ_B3, SQ_B4, SQ_C3}},

                   {PAWN, SQ_D4, {SQ_D5}},
                   {PAWN, SQ_E5, {SQ_D6, SQ_F6}},
                   {PAWN, SQ_H6, {SQ_H7}},
               });
}

TEST(advanced, test2) {
  PositionTest("5k2/8/3n4/2Q3b1/8/2p2N2/PP6/4KB2 w - - 0 1",
               {
                   {KING, SQ_E1, {SQ_D1, SQ_E2, SQ_F2}},
                   {KNIGHT, SQ_F3, {SQ_G1, SQ_H2, SQ_H4, SQ_G5, SQ_E5, SQ_D4, SQ_D2}},
                   {PAWN, SQ_A2, {SQ_A3, SQ_A4}},
                   {PAWN, SQ_B2, {SQ_B3, SQ_B4, SQ_C3}},
                   {BISHOP, SQ_F1, {SQ_G2, SQ_H3, SQ_E2, SQ_D3, SQ_C4, SQ_B5, SQ_A6}},
                   {QUEEN, SQ_C5, {SQ_A5, SQ_B5, SQ_D5, SQ_E5, SQ_F5, SQ_G5, SQ_C3, SQ_C4, SQ_C6, SQ_C7,
                                SQ_C8, SQ_A7, SQ_B6, SQ_D4, SQ_E3, SQ_F2, SQ_G1, SQ_A3, SQ_B4, SQ_D6}},
               });
}

TEST(advanced, test3) {
  PositionTest("rnbqkbnr/pp2pppp/8/3p4/1PpPP3/2P5/P4PPP/RNBQKBNR b KQkq b3 0 4",
               {
                   {KNIGHT, SQ_B8, {SQ_A6, SQ_C6, SQ_D7}},
                   {BISHOP, SQ_C8, {SQ_D7, SQ_E6, SQ_F5, SQ_G4, SQ_H3}},
                   {QUEEN, SQ_D8, {SQ_D7, SQ_D6, SQ_C7, SQ_B6, SQ_A5}},
                   {KING, SQ_E8, {SQ_D7}},
                   {KNIGHT, SQ_G8, {SQ_F6, SQ_H6}},

                   {PAWN, SQ_A7, {SQ_A6, SQ_A5}},
                   {PAWN, SQ_B7, {SQ_B6, SQ_B5}},
                   {PAWN, SQ_C4, {SQ_B3}},
                   {PAWN, SQ_D5, {SQ_E4}},
                   {PAWN, SQ_E7, {SQ_E6, SQ_E5}},
                   {PAWN, SQ_F7, {SQ_F6, SQ_F5}},
                   {PAWN, SQ_G7, {SQ_G6, SQ_G5}},
                   {PAWN, SQ_H7, {SQ_H6, SQ_H5}},
               });
}

}  // namespace Leslie::test
