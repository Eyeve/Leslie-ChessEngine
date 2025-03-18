#include <gtest/gtest.h>

#include <unordered_set>

#include "position.h"

TEST(rook_moves, test1) {
  leslie::Position position("8/8/8/1p1R1P2/8/1P6/8/3b4 w - - 0 1");
  std::vector<leslie::Move> actual;
  position.GetMoves(actual);
  std::unordered_set<leslie::Move, leslie::MoveHash> expected = {
      leslie::Move(leslie::PieceType::kRook, leslie::Square::kD5,leslie::Square::kD1),
      leslie::Move(leslie::PieceType::kRook, leslie::Square::kD5,leslie::Square::kD2),
      leslie::Move(leslie::PieceType::kRook, leslie::Square::kD5,leslie::Square::kD3),
      leslie::Move(leslie::PieceType::kRook, leslie::Square::kD5,leslie::Square::kD4),
      leslie::Move(leslie::PieceType::kRook, leslie::Square::kD5,leslie::Square::kD6),
      leslie::Move(leslie::PieceType::kRook, leslie::Square::kD5,leslie::Square::kD7),
      leslie::Move(leslie::PieceType::kRook, leslie::Square::kD5,leslie::Square::kD8),
      leslie::Move(leslie::PieceType::kRook, leslie::Square::kD5,leslie::Square::kB5),
      leslie::Move(leslie::PieceType::kRook, leslie::Square::kD5,leslie::Square::kC5),
      leslie::Move(leslie::PieceType::kRook, leslie::Square::kD5,leslie::Square::kE5),
      leslie::Move(leslie::PieceType::kPawn, leslie::Square::kF5,leslie::Square::kF6),
      leslie::Move(leslie::PieceType::kPawn, leslie::Square::kB3,leslie::Square::kB4),

  };
  EXPECT_EQ(expected.size(), actual.size());
  for (leslie::Move move : actual) expected.erase(move);
  EXPECT_TRUE(expected.empty());
}

TEST(rook_moves, test2) {
  leslie::Position position("8/8/8/8/8/6N1/1P4rN/6r1 b - - 0 1");
  std::vector<leslie::Move> actual;
  position.GetMoves(actual);
  std::unordered_set<leslie::Move, leslie::MoveHash> expected = {
      leslie::Move(leslie::PieceType::kRook, leslie::Square::kG2,leslie::Square::kG3),
      leslie::Move(leslie::PieceType::kRook, leslie::Square::kG2,leslie::Square::kH2),
      leslie::Move(leslie::PieceType::kRook, leslie::Square::kG2,leslie::Square::kB2),
      leslie::Move(leslie::PieceType::kRook, leslie::Square::kG2,leslie::Square::kC2),
      leslie::Move(leslie::PieceType::kRook, leslie::Square::kG2,leslie::Square::kD2),
      leslie::Move(leslie::PieceType::kRook, leslie::Square::kG2,leslie::Square::kE2),
      leslie::Move(leslie::PieceType::kRook, leslie::Square::kG2,leslie::Square::kF2),
      leslie::Move(leslie::PieceType::kRook, leslie::Square::kG1,leslie::Square::kA1),
      leslie::Move(leslie::PieceType::kRook, leslie::Square::kG1,leslie::Square::kB1),
      leslie::Move(leslie::PieceType::kRook, leslie::Square::kG1,leslie::Square::kC1),
      leslie::Move(leslie::PieceType::kRook, leslie::Square::kG1,leslie::Square::kD1),
      leslie::Move(leslie::PieceType::kRook, leslie::Square::kG1,leslie::Square::kE1),
      leslie::Move(leslie::PieceType::kRook, leslie::Square::kG1,leslie::Square::kF1),
      leslie::Move(leslie::PieceType::kRook, leslie::Square::kG1,leslie::Square::kH1),
  };
  EXPECT_EQ(expected.size(), actual.size());
  for (leslie::Move move : actual) expected.erase(move);
  EXPECT_TRUE(expected.empty());
}