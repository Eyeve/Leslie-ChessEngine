#include <gtest/gtest.h>

#include <unordered_set>

#include "position.h"

TEST(bishop_moves, test1) {
  leslie::Position position("8/8/8/r5P1/5P2/8/3B4/2n5 w - - 0 1");
  std::vector<leslie::Move> actual;
  position.GetMoves(actual);
  std::unordered_set<leslie::Move, leslie::MoveHash> expected = {
      leslie::Move(leslie::PieceType::kBishop, leslie::Square::kD2,leslie::Square::kC1),
      leslie::Move(leslie::PieceType::kBishop, leslie::Square::kD2,leslie::Square::kE1),
      leslie::Move(leslie::PieceType::kBishop, leslie::Square::kD2,leslie::Square::kC3),
      leslie::Move(leslie::PieceType::kBishop, leslie::Square::kD2,leslie::Square::kB4),
      leslie::Move(leslie::PieceType::kBishop, leslie::Square::kD2,leslie::Square::kA5),
      leslie::Move(leslie::PieceType::kBishop, leslie::Square::kD2,leslie::Square::kE3),
      leslie::Move(leslie::PieceType::kPawn, leslie::Square::kF4,leslie::Square::kF5),
      leslie::Move(leslie::PieceType::kPawn, leslie::Square::kG5,leslie::Square::kG6),
  };
  EXPECT_EQ(expected.size(), actual.size());
  for (leslie::Move move : actual) expected.erase(move);
  EXPECT_TRUE(expected.empty());
}
