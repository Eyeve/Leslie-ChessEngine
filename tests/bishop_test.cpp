#include <gtest/gtest.h>

#include "tests.h"

namespace leslie::test {

TEST(bishop, test1) {
  PositionTest("8/3k4/8/r5P1/5P2/8/3B1K2/2n5 w - - 0 1",
               {
                   {PieceType::kKing,
                    Square::kF2,
                    {Square::kF1, Square::kG1, Square::kG2, Square::kG3,
                     Square::kF3, Square::kE3, Square::kE1}},
                   {PieceType::kBishop,
                    Square::kD2,
                    {Square::kC1, Square::kE1, Square::kC3, Square::kB4,
                     Square::kA5, Square::kE3}},

                   {PieceType::kPawn, Square::kF4, {Square::kF5}},
                   {PieceType::kPawn, Square::kG5, {Square::kG6}},
               });
}

}  // namespace leslie::test
