#include "position.h"
#include <unordered_set>

#include <gtest/gtest.h>

TEST(king_moves, test1)
{
	Leslie::Position position("K7/8/8/8/8/8/8/8 w - - 0 1");
	std::vector< Leslie::Move > actual;
	position.get_moves(actual);
	std::unordered_set< Leslie::Move, Leslie::MoveHash > expected = {
		Leslie::Move(Leslie::PieceType::KING, Leslie::FileA & Leslie::Rank8, Leslie::FileB & Leslie::Rank8),
		Leslie::Move(Leslie::PieceType::KING, Leslie::FileA & Leslie::Rank8, Leslie::FileB & Leslie::Rank7),
		Leslie::Move(Leslie::PieceType::KING, Leslie::FileA & Leslie::Rank8, Leslie::FileA & Leslie::Rank7),
	};
	EXPECT_EQ(expected.size(), actual.size());
	for (Leslie::Move move : actual)
		expected.erase(move);
	EXPECT_TRUE(expected.empty());
}

TEST(king_moves, test2)
{
	Leslie::Position position("8/8/8/3k4/3P4/8/8/8 b - - 0 1");
	std::vector< Leslie::Move > actual;
	position.get_moves(actual);
	std::unordered_set< Leslie::Move, Leslie::MoveHash > expected = {
		Leslie::Move(Leslie::PieceType::KING, Leslie::Square::D5, Leslie::Square::C6),
		Leslie::Move(Leslie::PieceType::KING, Leslie::Square::D5, Leslie::Square::D6),
		Leslie::Move(Leslie::PieceType::KING, Leslie::Square::D5, Leslie::Square::E6),
		Leslie::Move(Leslie::PieceType::KING, Leslie::Square::D5, Leslie::Square::C5),
		Leslie::Move(Leslie::PieceType::KING, Leslie::Square::D5, Leslie::Square::E5),
		Leslie::Move(Leslie::PieceType::KING, Leslie::Square::D5, Leslie::Square::C4),
		Leslie::Move(Leslie::PieceType::KING, Leslie::Square::D5, Leslie::Square::D4),
		Leslie::Move(Leslie::PieceType::KING, Leslie::Square::D5, Leslie::Square::E4),
	};
	EXPECT_EQ(expected.size(), actual.size());
	for (Leslie::Move move : actual)
		expected.erase(move);
	EXPECT_TRUE(expected.empty());
}
