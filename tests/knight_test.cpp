#include <gtest/gtest.h>

#include "tests.h"

namespace leslie::test {

TEST(knight, test1) {
  PositionTest("N7/8/8/8/8/8/8/8 w - - 0 1",
               {{PieceType::kKnight, Square::kA8, {Square::kC7, Square::kB6}}});
}

TEST(knight, test2) {
  PositionTest("8/8/6n1/N7/5P2/8/8/8 b - - 0 1",
               {{PieceType::kKnight,
                 Square::kG6,
                 {Square::kH8, Square::kF8, Square::kE7, Square::kE5,
                  Square::kF4, Square::kH4}}});
}

}  // namespace leslie::test
