#include <gtest/gtest.h>

#include "tests.h"

namespace leslie::test {

TEST(bishop, test1) {
  PositionTest("8/8/8/r5P1/5P2/8/3B4/2n5 w - - 0 1",
               {
                   {PieceType::kBishop,
                    Square::kD2,
                    {Square::kC1, Square::kE1, Square::kC3, Square::kB4,
                     Square::kA5, Square::kE3}},

                   {PieceType::kPawn, Square::kF4, {Square::kF5}},
                   {PieceType::kPawn, Square::kG5, {Square::kG6}},
               });
}

}  // namespace leslie::test
