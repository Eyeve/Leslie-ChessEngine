#include <gtest/gtest.h>

#include "tests.h"

namespace leslie::test {

TEST(king, test1) {
  PositionTest("K7/8/8/8/8/8/8/8 w - - 0 1",
               {
                   {PieceType::kKing,
                    Square::kA8,
                    {Square::kB8, Square::kB7, Square::kA7}},
               });
}

TEST(king, test2) {
  PositionTest("8/8/8/3k4/3P4/8/8/8 b - - 0 1",
               {
                   {PieceType::kKing,
                    Square::kD5,
                    {Square::kC6, Square::kD6, Square::kE6, Square::kC5,
                     Square::kE5, Square::kC4, Square::kD4, Square::kE4}},
               });
}

}  // namespace leslie::test
