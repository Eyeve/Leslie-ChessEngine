#include "nnue/nnue.h"

#include <gtest/gtest.h>

#include "position.h"
#include "tests.h"

TEST(nnue, test_est) {
  leslie::Position pos1("");
  leslie::Position pos2("");

  leslie::nnue::Nnue nnue(pos1);

  int16_t est1 = nnue.Eval();
  int16_t est2 = nnue.Eval(pos2);

//  EXPECT_TRUE(est1 > est2);
}

TEST(nnue, test_update) {
  leslie::Position pos("");
  leslie::Position pos_after_move("");
  leslie::Move move();

  leslie::nnue::Nnue nnue(pos);
  int16_t est1 = nnue.Update(reinterpret_cast<leslie::Move&>(move));
  int16_t est2 = nnue.Eval(pos_after_move);

//  EXPECT_EQ(est1, est2);

}