#include "nnue/nnue"

#include <gtest/gtest.h>

#include "position.h"
#include "tests.h"

TEST(nnue, test_est) {
  // leslie::Position pos1("");
  // leslie::Position pos2("");

  // leslie::nnue::Nnue nnue(pos1);

  // int16_t est1 = nnue.Eval();
  // int16_t est2 = nnue.Eval(pos2);

  //  EXPECT_TRUE(est1 > est2);
}

TEST(nnue, test_update) {
  // leslie::Position pos("");
  // leslie::Position pos_after_move("");
  // leslie::Move move;

  // leslie::nnue::Nnue nnue(pos);
  // int16_t est1 = nnue.Update(reinterpret_cast<leslie::Move&>(move));
  // int16_t est2 = nnue.Eval(pos_after_move);

  //  EXPECT_EQ(est1, est2);
}

TEST(static_analysis, test_simple_anal1) {
  Leslie::Position position("r7/3n1p2/p4k1p/2pP2p1/6P1/5N1P/PP3P2/R3R1K1 w - - 2 28");
  EXPECT_EQ(600, position.GetSimpleEstimation());
}

TEST(static_analysis, test_simple_anal2) {
  Leslie::Position position("r7/3n1p2/p4k1p/2pP2p1/6P1/5N1P/PP3P2/R3R1K1 b - - 2 28");
  EXPECT_EQ(600, position.GetSimpleEstimation());
}

TEST(static_analysis, test_simple_anal3) {
  Leslie::Position position("rn1q1rk1/4ppBp/2p3p1/p4b2/Pp1P4/1B3N2/1PPnN1PP/R4RK1 w - - 0 15");
  EXPECT_EQ(-690, position.GetSimpleEstimation());
}
