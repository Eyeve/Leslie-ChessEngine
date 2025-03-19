#include <gtest/gtest.h>

#include "tests.h"

namespace leslie::test {

TEST(pawn, test1) {
  PositionTest("8/p7/1P5N/8/8/8/8/8 b - - 0 1",
               {{PieceType::kPawn,
                 Square::kA7,
                 {Square::kA6, Square::kB6, Square::kA5}}});
}

TEST(pawn, test2) {
  PositionTest("8/8/3pbn2/4P3/2p5/3P4/1PP5/8 w - - 0 1",
               {{
                    PieceType::kPawn,
                    Square::kB2,
                    {Square::kB3, Square::kB4},
                },
                {
                    PieceType::kPawn,
                    Square::kC2,
                    {Square::kC3},
                },
                {
                    PieceType::kPawn,
                    Square::kD3,
                    {Square::kC4, Square::kD4},
                },
                {
                    PieceType::kPawn,
                    Square::kE5,
                    {Square::kD6, Square::kF6},
                }});
}

TEST(pawn, test3) {
  PositionTest("8/3p1pp1/2Q2N2/2p5/N6p/B5P1/p7/7R b - - 0 1",
               {{
                    PieceType::kPawn,
                    Square::kA2,
                    {Square::kA1},
                },
                {
                    PieceType::kPawn,
                    Square::kC5,
                    {Square::kC4},
                },
                {
                    PieceType::kPawn,
                    Square::kD7,
                    {Square::kD6, Square::kD5, Square::kC6},
                },
                {
                    PieceType::kPawn,
                    Square::kG7,
                    {Square::kG6, Square::kG5, Square::kF6},
                },
                {
                    PieceType::kPawn,
                    Square::kH4,
                    {Square::kG3, Square::kH3},
                }});
}

}  // namespace leslie::test
