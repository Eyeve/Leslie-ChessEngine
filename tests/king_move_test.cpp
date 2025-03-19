#include <gtest/gtest.h>

#include <unordered_set>

#include "position.h"

TEST(king_moves, test1) {
  leslie::Position position("K7/8/8/8/8/8/8/8 w - - 0 1");
  std::vector<leslie::Move> actual;
  position.GetMoves(actual);
  std::unordered_set<leslie::Move, leslie::MoveHash> expected = {
      leslie::Move(leslie::PieceType::kKing, leslie::kFileA & leslie::kRank8,
                   leslie::kFileB & leslie::kRank8),
      leslie::Move(leslie::PieceType::kKing, leslie::kFileA & leslie::kRank8,
                   leslie::kFileB & leslie::kRank7),
      leslie::Move(leslie::PieceType::kKing, leslie::kFileA & leslie::kRank8,
                   leslie::kFileA & leslie::kRank7),
  };
  EXPECT_EQ(expected.size(), actual.size());
  for (leslie::Move move : actual) expected.erase(move);
  EXPECT_TRUE(expected.empty());
}

TEST(king_moves, test2) {
  leslie::Position position("8/8/8/3k4/3P4/8/8/8 b - - 0 1");
  std::vector<leslie::Move> actual;
  position.GetMoves(actual);
  std::unordered_set<leslie::Move, leslie::MoveHash> expected = {
      leslie::Move(leslie::PieceType::kKing, leslie::Square::kD5,
                   leslie::Square::kC6),
      leslie::Move(leslie::PieceType::kKing, leslie::Square::kD5,
                   leslie::Square::kD6),
      leslie::Move(leslie::PieceType::kKing, leslie::Square::kD5,
                   leslie::Square::kE6),
      leslie::Move(leslie::PieceType::kKing, leslie::Square::kD5,
                   leslie::Square::kC5),
      leslie::Move(leslie::PieceType::kKing, leslie::Square::kD5,
                   leslie::Square::kE5),
      leslie::Move(leslie::PieceType::kKing, leslie::Square::kD5,
                   leslie::Square::kC4),
      leslie::Move(leslie::PieceType::kKing, leslie::Square::kD5,
                   leslie::Square::kD4),
      leslie::Move(leslie::PieceType::kKing, leslie::Square::kD5,
                   leslie::Square::kE4),
  };
  EXPECT_EQ(expected.size(), actual.size());
  for (leslie::Move move : actual) expected.erase(move);
  EXPECT_TRUE(expected.empty());
}