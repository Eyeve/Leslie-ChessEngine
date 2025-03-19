#include <gtest/gtest.h>

#include "tests.h"

namespace leslie::test {

TEST(rook, test1) {
  PositionTest(
      "8/8/8/1p1R1P2/8/1P6/8/3b4 w - - 0 1",
      {
          {PieceType::kRook,
           Square::kD5,
           {Square::kD1, Square::kD2, Square::kD3, Square::kD4, Square::kD6,
            Square::kD7, Square::kD8, Square::kB5, Square::kC5, Square::kE5}},

          {PieceType::kPawn, Square::kF5, {Square::kF6}},
          {PieceType::kPawn, Square::kB3, {Square::kB4}},
      });
}

TEST(rook, test2) {
  PositionTest("8/8/8/8/8/6N1/1P4rN/6r1 b - - 0 1",
               {
                   {PieceType::kRook,
                    Square::kG2,
                    {Square::kG3, Square::kH2, Square::kB2, Square::kC2,
                     Square::kD2, Square::kE2, Square::kF2}},
                   {PieceType::kRook,
                    Square::kG1,
                    {Square::kA1, Square::kB1, Square::kC1, Square::kD1,
                     Square::kE1, Square::kF1, Square::kH1}},
               });
}

}  // namespace leslie::test
