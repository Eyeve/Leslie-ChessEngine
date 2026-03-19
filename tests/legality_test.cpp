#include <gtest/gtest.h>

#include "tests.h"

namespace Leslie::Test {

TEST(legality_castling, allows_both_sides_when_valid) {
  const auto moves = CollectMoves("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");

  EXPECT_TRUE(HasMove(moves, Move(KING, SQ_E1, SQ_G1)));
  EXPECT_TRUE(HasMove(moves, Move(KING, SQ_E1, SQ_C1)));
}

TEST(legality_castling, blocks_kingside_when_path_attacked) {
  const auto moves = CollectMoves("4kr2/8/8/8/8/8/8/R3K2R w KQ - 0 1");

  EXPECT_FALSE(HasMove(moves, Move(KING, SQ_E1, SQ_G1)));
  EXPECT_TRUE(HasMove(moves, Move(KING, SQ_E1, SQ_C1)));
}

TEST(legality_castling, blocks_castling_when_king_in_check) {
  const auto moves = CollectMoves("k3r3/8/8/8/8/8/8/R3K2R w KQ - 0 1");

  EXPECT_FALSE(HasMove(moves, Move(KING, SQ_E1, SQ_G1)));
  EXPECT_FALSE(HasMove(moves, Move(KING, SQ_E1, SQ_C1)));
}

TEST(legality_castling, requires_rook_presence) {
  const auto moves = CollectMoves("4k3/8/8/8/8/8/8/4K3 w KQ - 0 1");

  EXPECT_FALSE(HasMove(moves, Move(KING, SQ_E1, SQ_G1)));
  EXPECT_FALSE(HasMove(moves, Move(KING, SQ_E1, SQ_C1)));
}

TEST(legality_en_passant, allows_when_legal) {
  const auto moves = CollectMoves("4k3/8/8/3pP3/8/8/8/4K3 w - d6 0 1");

  EXPECT_TRUE(HasMove(moves, Move(PAWN, SQ_E5, SQ_D6)));
}

TEST(legality_en_passant, blocks_when_exposes_king) {
  const auto moves = CollectMoves("k3r3/8/8/3pP3/8/8/8/4K3 w - d6 0 1");

  EXPECT_FALSE(HasMove(moves, Move(PAWN, SQ_E5, SQ_D6)));
}

}  // namespace Leslie::Test
