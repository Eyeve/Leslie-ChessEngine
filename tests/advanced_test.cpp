#include <gtest/gtest.h>

#include "tests.h"

namespace leslie::test {
TEST(advanced, test1) {
  PositionTest("8/nK6/2Nrpq1P/4P3/3P4/2r5/1P4N1/8 w - - 0 1",
               {
                   {PieceType::kKing,
                    Square::kB7,
                    {Square::kB6, Square::kA6, Square::kA7, Square::kA8,
                     Square::kB8, Square::kC8, Square::kC7}},

                   {PieceType::kKnight,
                    Square::kC6,
                    {Square::kB4, Square::kA5, Square::kA7, Square::kB8,
                     Square::kD8, Square::kE7}},

                   {PieceType::kKnight,
                    Square::kG2,
                    {Square::kE1, Square::kE3, Square::kF4, Square::kH4}},

                   {PieceType::kPawn,
                    Square::kB2,
                    {Square::kB3, Square::kB4, Square::kC3}},

                   {PieceType::kPawn, Square::kD4, {Square::kD5}},
                   {PieceType::kPawn, Square::kE5, {Square::kD6, Square::kF6}},
                   {PieceType::kPawn, Square::kH6, {Square::kH7}},
               });
}

}  // namespace leslie::test
