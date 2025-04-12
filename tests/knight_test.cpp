#include <gtest/gtest.h>

#include "tests.h"

namespace Leslie::test {

TEST(knight, test1) {
  PositionTest("N7/8/8/8/6k1/8/1K6/8 w - - 0 1",
               {{PieceType::kKing,
                 Square::kB2,
                 {Square::kA1, Square::kA2, Square::kA3, Square::kB1,
                  Square::kB3, Square::kC1, Square::kC2, Square::kC3}},
                {PieceType::kKnight, Square::kA8, {Square::kC7, Square::kB6}}});
}

TEST(knight, test2) {
  PositionTest("8/K7/6n1/N7/5P2/8/1k6/8 b - - 0 1",
               {{PieceType::kKing,
                 Square::kB2,
                 {Square::kA1, Square::kA2, Square::kA3, Square::kB1,
                  Square::kC1, Square::kC2, Square::kC3}},
                {PieceType::kKnight,
                 Square::kG6,
                 {Square::kH8, Square::kF8, Square::kE7, Square::kE5,
                  Square::kF4, Square::kH4}}});
}

}  // namespace leslie::test
