#include "position.h"
#include <unordered_set>

#include <gtest/gtest.h>

TEST(pawn_moves, test1)
{
	Leslie::Position position("8/p7/1P5N/8/8/8/8/8 b - - 0 1");
	std::vector< Leslie::Move > actual;
	position.get_moves(actual);
	std::unordered_set< Leslie::Move, Leslie::MoveHash > expected = {
		Leslie::Move(Leslie::PieceType::PAWN, Leslie::FileA & Leslie::Rank7, Leslie::FileA & Leslie::Rank6),
		Leslie::Move(Leslie::PieceType::PAWN, Leslie::FileA & Leslie::Rank7, Leslie::FileB & Leslie::Rank6),
		Leslie::Move(Leslie::PieceType::PAWN, Leslie::FileA & Leslie::Rank7, Leslie::FileA & Leslie::Rank5),
	};
	EXPECT_EQ(expected.size(), actual.size());
	for (Leslie::Move move : actual)
		expected.erase(move);
	EXPECT_TRUE(expected.empty());
}

TEST(pawn_moves, test2)
{
	Leslie::Position position("8/8/3pbn2/4P3/2p5/3P4/1PP5/8 w - - 0 1");
	std::vector< Leslie::Move > actual;
	position.get_moves(actual);
	std::unordered_set< Leslie::Move, Leslie::MoveHash > expected = {
		Leslie::Move(Leslie::PieceType::PAWN, Leslie::Square::B2, Leslie::Square::B3),
		Leslie::Move(Leslie::PieceType::PAWN, Leslie::Square::B2, Leslie::Square::B4),
		Leslie::Move(Leslie::PieceType::PAWN, Leslie::Square::C2, Leslie::Square::C3),
		Leslie::Move(Leslie::PieceType::PAWN, Leslie::Square::D3, Leslie::Square::C4),
		Leslie::Move(Leslie::PieceType::PAWN, Leslie::Square::D3, Leslie::Square::D4),
		Leslie::Move(Leslie::PieceType::PAWN, Leslie::Square::E5, Leslie::Square::D6),
		Leslie::Move(Leslie::PieceType::PAWN, Leslie::Square::E5, Leslie::Square::F6),
	};
	EXPECT_EQ(expected.size(), actual.size());
	for (Leslie::Move move : actual)
		expected.erase(move);
	EXPECT_TRUE(expected.empty());
}

TEST(pawn_moves, test3)
{
	Leslie::Position position("8/3p1pp1/2Q2N2/2p5/N6p/B5P1/p7/7R b - - 0 1");
	std::vector< Leslie::Move > actual;
	position.get_moves(actual);
	std::unordered_set< Leslie::Move, Leslie::MoveHash > expected = {
		Leslie::Move(Leslie::PieceType::PAWN, Leslie::Square::A2, Leslie::Square::A1),
		Leslie::Move(Leslie::PieceType::PAWN, Leslie::Square::C5, Leslie::Square::C4),
		Leslie::Move(Leslie::PieceType::PAWN, Leslie::Square::D7, Leslie::Square::D6),
		Leslie::Move(Leslie::PieceType::PAWN, Leslie::Square::D7, Leslie::Square::D5),
		Leslie::Move(Leslie::PieceType::PAWN, Leslie::Square::D7, Leslie::Square::C6),
		Leslie::Move(Leslie::PieceType::PAWN, Leslie::Square::G7, Leslie::Square::G6),
		Leslie::Move(Leslie::PieceType::PAWN, Leslie::Square::G7, Leslie::Square::G5),
		Leslie::Move(Leslie::PieceType::PAWN, Leslie::Square::G7, Leslie::Square::F6),
		Leslie::Move(Leslie::PieceType::PAWN, Leslie::Square::H4, Leslie::Square::G3),
		Leslie::Move(Leslie::PieceType::PAWN, Leslie::Square::H4, Leslie::Square::H3),
	};
	EXPECT_EQ(expected.size(), actual.size());
	for (Leslie::Move move : actual)
		expected.erase(move);
	EXPECT_TRUE(expected.empty());
}