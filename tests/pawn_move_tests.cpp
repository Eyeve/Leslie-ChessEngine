#include <gtest/gtest.h>

#include <unordered_set>

#include "position.h"

TEST(pawn_moves, test1) {
  leslie::Position position("8/p7/1P5N/8/8/8/8/8 b - - 0 1");
  std::vector<leslie::Move> actual;
  position.GetMoves(actual);
  std::unordered_set<leslie::Move, leslie::MoveHash> expected = {
      leslie::Move(leslie::PieceType::kPawn, leslie::kFileA & leslie::kRank7,
                   leslie::kFileA & leslie::kRank6),
      leslie::Move(leslie::PieceType::kPawn, leslie::kFileA & leslie::kRank7,
                   leslie::kFileB & leslie::kRank6),
      leslie::Move(leslie::PieceType::kPawn, leslie::kFileA & leslie::kRank7,
                   leslie::kFileA & leslie::kRank5),
  };
  EXPECT_EQ(expected.size(), actual.size());
  for (leslie::Move move : actual) expected.erase(move);
  EXPECT_TRUE(expected.empty());
}

TEST(pawn_moves, test2) {
  leslie::Position position("8/8/3pbn2/4P3/2p5/3P4/1PP5/8 w - - 0 1");
  std::vector<leslie::Move> actual;
  position.GetMoves(actual);
  std::unordered_set<leslie::Move, leslie::MoveHash> expected = {
      leslie::Move(leslie::PieceType::kPawn, leslie::Square::kB2,
                   leslie::Square::kB3),
      leslie::Move(leslie::PieceType::kPawn, leslie::Square::kB2,
                   leslie::Square::kB4),
      leslie::Move(leslie::PieceType::kPawn, leslie::Square::kC2,
                   leslie::Square::kC3),
      leslie::Move(leslie::PieceType::kPawn, leslie::Square::kD3,
                   leslie::Square::kC4),
      leslie::Move(leslie::PieceType::kPawn, leslie::Square::kD3,
                   leslie::Square::kD4),
      leslie::Move(leslie::PieceType::kPawn, leslie::Square::kE5,
                   leslie::Square::kD6),
      leslie::Move(leslie::PieceType::kPawn, leslie::Square::kE5,
                   leslie::Square::kF6),
  };
  EXPECT_EQ(expected.size(), actual.size());
  for (leslie::Move move : actual) expected.erase(move);
  EXPECT_TRUE(expected.empty());
}

TEST(pawn_moves, test3) {
  leslie::Position position("8/3p1pp1/2Q2N2/2p5/N6p/B5P1/p7/7R b - - 0 1");
  std::vector<leslie::Move> actual;
  position.GetMoves(actual);
  std::unordered_set<leslie::Move, leslie::MoveHash> expected = {
      leslie::Move(leslie::PieceType::kPawn, leslie::Square::kA2,
                   leslie::Square::kA1),
      leslie::Move(leslie::PieceType::kPawn, leslie::Square::kC5,
                   leslie::Square::kC4),
      leslie::Move(leslie::PieceType::kPawn, leslie::Square::kD7,
                   leslie::Square::kD6),
      leslie::Move(leslie::PieceType::kPawn, leslie::Square::kD7,
                   leslie::Square::kD5),
      leslie::Move(leslie::PieceType::kPawn, leslie::Square::kD7,
                   leslie::Square::kC6),
      leslie::Move(leslie::PieceType::kPawn, leslie::Square::kG7,
                   leslie::Square::kG6),
      leslie::Move(leslie::PieceType::kPawn, leslie::Square::kG7,
                   leslie::Square::kG5),
      leslie::Move(leslie::PieceType::kPawn, leslie::Square::kG7,
                   leslie::Square::kF6),
      leslie::Move(leslie::PieceType::kPawn, leslie::Square::kH4,
                   leslie::Square::kG3),
      leslie::Move(leslie::PieceType::kPawn, leslie::Square::kH4,
                   leslie::Square::kH3),
  };
  EXPECT_EQ(expected.size(), actual.size());
  for (leslie::Move move : actual) expected.erase(move);
  EXPECT_TRUE(expected.empty());
}