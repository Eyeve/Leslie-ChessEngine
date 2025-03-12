#include "position.h"
#include <unordered_set>

#include <gtest/gtest.h>

TEST(knight_moves, test1)
{
	Leslie::Position position("N7/8/8/8/8/8/8/8 w - - 0 1");
	std::vector< Leslie::Move > actual;
	position.get_moves(actual);
	std::unordered_set< Leslie::Move, Leslie::MoveHash > expected = {
		Leslie::Move(Leslie::PieceType::KNIGHT, Leslie::FileA & Leslie::Rank8, Leslie::FileC & Leslie::Rank7),
		Leslie::Move(Leslie::PieceType::KNIGHT, Leslie::FileA & Leslie::Rank8, Leslie::FileB & Leslie::Rank6),
	};
	EXPECT_EQ(expected.size(), actual.size());
	for (Leslie::Move move : actual)
		expected.erase(move);
	EXPECT_TRUE(expected.empty());
}

TEST(knight_moves, test2)
{
	Leslie::Position position("8/8/6n1/N7/5P2/8/8/8 b - - 0 1");
	std::vector< Leslie::Move > actual;
	position.get_moves(actual);
	std::unordered_set< Leslie::Move, Leslie::MoveHash > expected = {
		Leslie::Move(Leslie::PieceType::KNIGHT, Leslie::Square::G6, Leslie::Square::H8),
		Leslie::Move(Leslie::PieceType::KNIGHT, Leslie::Square::G6, Leslie::Square::F8),
		Leslie::Move(Leslie::PieceType::KNIGHT, Leslie::Square::G6, Leslie::Square::E7),
		Leslie::Move(Leslie::PieceType::KNIGHT, Leslie::Square::G6, Leslie::Square::E5),
		Leslie::Move(Leslie::PieceType::KNIGHT, Leslie::Square::G6, Leslie::Square::F4),
		Leslie::Move(Leslie::PieceType::KNIGHT, Leslie::Square::G6, Leslie::Square::H4),
	};
	EXPECT_EQ(expected.size(), actual.size());
	for (Leslie::Move move : actual)
		expected.erase(move);
	EXPECT_TRUE(expected.empty());
}
