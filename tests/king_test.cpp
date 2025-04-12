#include <gtest/gtest.h>

#include "tests.h"

namespace Leslie::Test {

TEST(king, test1) {
  PositionTest("K7/8/8/8/8/4k3/8/8 w - - 0 1",
               {
                   {KING, SQ_A8, {SQ_B8, SQ_B7, SQ_A7}},
               });
}

TEST(king, test2) {
  PositionTest("8/8/8/3k4/3P4/8/K7/8 b - - 0 1",
               {
                   {KING, SQ_D5, {SQ_C6, SQ_D6, SQ_E6, SQ_C4, SQ_D4, SQ_E4}},
               });
}

TEST(king, test3) {
  PositionTest("1r1r4/6k1/8/8/8/2K5/8/8 w - - 0 1",
               {
                   {KING, SQ_C3, {SQ_C2, SQ_C4}},
               });
}

}  // namespace Leslie::test
