#include <gtest/gtest.h>

#include <unordered_set>

#include "position.h"

TEST(KNP_moves, test1) {
  leslie::Position position("8/nK6/2Nrpq1P/4P3/3P4/2r5/1P4N1/8 w - - 0 1");
  std::vector<leslie::Move> actual;
  position.GetMoves(actual);
  std::unordered_set<leslie::Move, leslie::MoveHash> expected = {
      leslie::Move(leslie::PieceType::kKing, leslie::Square::kB7,
                   leslie::Square::kB6),
      leslie::Move(leslie::PieceType::kKing, leslie::Square::kB7,
                   leslie::Square::kA6),
      leslie::Move(leslie::PieceType::kKing, leslie::Square::kB7,
                   leslie::Square::kA7),
      leslie::Move(leslie::PieceType::kKing, leslie::Square::kB7,
                   leslie::Square::kA8),
      leslie::Move(leslie::PieceType::kKing, leslie::Square::kB7,
                   leslie::Square::kB8),
      leslie::Move(leslie::PieceType::kKing, leslie::Square::kB7,
                   leslie::Square::kC8),
      leslie::Move(leslie::PieceType::kKing, leslie::Square::kB7,
                   leslie::Square::kC7),
      leslie::Move(leslie::PieceType::kKnight, leslie::Square::kC6,
                   leslie::Square::kB4),
      leslie::Move(leslie::PieceType::kKnight, leslie::Square::kC6,
                   leslie::Square::kA5),
      leslie::Move(leslie::PieceType::kKnight, leslie::Square::kC6,
                   leslie::Square::kA7),
      leslie::Move(leslie::PieceType::kKnight, leslie::Square::kC6,
                   leslie::Square::kB8),
      leslie::Move(leslie::PieceType::kKnight, leslie::Square::kC6,
                   leslie::Square::kD8),
      leslie::Move(leslie::PieceType::kKnight, leslie::Square::kC6,
                   leslie::Square::kE7),
      leslie::Move(leslie::PieceType::kKnight, leslie::Square::kG2,
                   leslie::Square::kE1),
      leslie::Move(leslie::PieceType::kKnight, leslie::Square::kG2,
                   leslie::Square::kE3),
      leslie::Move(leslie::PieceType::kKnight, leslie::Square::kG2,
                   leslie::Square::kF4),
      leslie::Move(leslie::PieceType::kKnight, leslie::Square::kG2,
                   leslie::Square::kH4),
      leslie::Move(leslie::PieceType::kPawn, leslie::Square::kB2,
                   leslie::Square::kB3),
      leslie::Move(leslie::PieceType::kPawn, leslie::Square::kB2,
                   leslie::Square::kB4),
      leslie::Move(leslie::PieceType::kPawn, leslie::Square::kB2,
                   leslie::Square::kC3),
      leslie::Move(leslie::PieceType::kPawn, leslie::Square::kD4,
                   leslie::Square::kD5),
      leslie::Move(leslie::PieceType::kPawn, leslie::Square::kE5,
                   leslie::Square::kD6),
      leslie::Move(leslie::PieceType::kPawn, leslie::Square::kE5,
                   leslie::Square::kF6),
      leslie::Move(leslie::PieceType::kPawn, leslie::Square::kH6,
                   leslie::Square::kH7),

  };
  EXPECT_EQ(expected.size(), actual.size());
  for (leslie::Move move : actual) expected.erase(move);
  EXPECT_TRUE(expected.empty());
}