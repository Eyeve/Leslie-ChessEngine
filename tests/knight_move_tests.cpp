#include <gtest/gtest.h>

#include <unordered_set>

#include "position.h"

TEST(knight_moves, test1) {
  leslie::Position position("N7/8/8/8/8/8/8/8 w - - 0 1");
  std::vector<leslie::Move> actual;
  position.GetMoves(actual);
  std::unordered_set<leslie::Move, leslie::MoveHash> expected = {
      leslie::Move(leslie::PieceType::kKnight, leslie::kFileA & leslie::kRank8,
                   leslie::kFileC & leslie::kRank7),
      leslie::Move(leslie::PieceType::kKnight, leslie::kFileA & leslie::kRank8,
                   leslie::kFileB & leslie::kRank6),
  };
  EXPECT_EQ(expected.size(), actual.size());
  for (leslie::Move move : actual) expected.erase(move);
  EXPECT_TRUE(expected.empty());
}

TEST(knight_moves, test2) {
  leslie::Position position("8/8/6n1/N7/5P2/8/8/8 b - - 0 1");
  std::vector<leslie::Move> actual;
  position.GetMoves(actual);
  std::unordered_set<leslie::Move, leslie::MoveHash> expected = {
      leslie::Move(leslie::PieceType::kKnight, leslie::Square::kG6,
                   leslie::Square::kH8),
      leslie::Move(leslie::PieceType::kKnight, leslie::Square::kG6,
                   leslie::Square::kF8),
      leslie::Move(leslie::PieceType::kKnight, leslie::Square::kG6,
                   leslie::Square::kE7),
      leslie::Move(leslie::PieceType::kKnight, leslie::Square::kG6,
                   leslie::Square::kE5),
      leslie::Move(leslie::PieceType::kKnight, leslie::Square::kG6,
                   leslie::Square::kF4),
      leslie::Move(leslie::PieceType::kKnight, leslie::Square::kG6,
                   leslie::Square::kH4),
  };
  EXPECT_EQ(expected.size(), actual.size());
  for (leslie::Move move : actual) expected.erase(move);
  EXPECT_TRUE(expected.empty());
}