#include "position.h"
#include <unordered_set>

#include <gtest/gtest.h>

TEST(KNP_moves, test1)
{
	Leslie::Position position("8/nK6/2Nrpq1P/4P3/3P4/2r5/1P4N1/8 w - - 0 1");
	std::vector< Leslie::Move > actual;
	position.get_moves(actual);
	std::unordered_set< Leslie::Move, Leslie::MoveHash > expected = {
		Leslie::Move(Leslie::PieceType::KING, Leslie::Square::B7, Leslie::Square::B6),
		Leslie::Move(Leslie::PieceType::KING, Leslie::Square::B7, Leslie::Square::A6),
		Leslie::Move(Leslie::PieceType::KING, Leslie::Square::B7, Leslie::Square::A7),
		Leslie::Move(Leslie::PieceType::KING, Leslie::Square::B7, Leslie::Square::A8),
		Leslie::Move(Leslie::PieceType::KING, Leslie::Square::B7, Leslie::Square::B8),
		Leslie::Move(Leslie::PieceType::KING, Leslie::Square::B7, Leslie::Square::C8),
		Leslie::Move(Leslie::PieceType::KING, Leslie::Square::B7, Leslie::Square::C7),
		Leslie::Move(Leslie::PieceType::KNIGHT, Leslie::Square::C6, Leslie::Square::B4),
		Leslie::Move(Leslie::PieceType::KNIGHT, Leslie::Square::C6, Leslie::Square::A5),
		Leslie::Move(Leslie::PieceType::KNIGHT, Leslie::Square::C6, Leslie::Square::A7),
		Leslie::Move(Leslie::PieceType::KNIGHT, Leslie::Square::C6, Leslie::Square::B8),
		Leslie::Move(Leslie::PieceType::KNIGHT, Leslie::Square::C6, Leslie::Square::D8),
		Leslie::Move(Leslie::PieceType::KNIGHT, Leslie::Square::C6, Leslie::Square::E7),
		Leslie::Move(Leslie::PieceType::KNIGHT, Leslie::Square::G2, Leslie::Square::E1),
		Leslie::Move(Leslie::PieceType::KNIGHT, Leslie::Square::G2, Leslie::Square::E3),
		Leslie::Move(Leslie::PieceType::KNIGHT, Leslie::Square::G2, Leslie::Square::F4),
		Leslie::Move(Leslie::PieceType::KNIGHT, Leslie::Square::G2, Leslie::Square::H4),
		Leslie::Move(Leslie::PieceType::PAWN, Leslie::Square::B2, Leslie::Square::B3),
		Leslie::Move(Leslie::PieceType::PAWN, Leslie::Square::B2, Leslie::Square::B4),
		Leslie::Move(Leslie::PieceType::PAWN, Leslie::Square::B2, Leslie::Square::C3),
		Leslie::Move(Leslie::PieceType::PAWN, Leslie::Square::D4, Leslie::Square::D5),
		Leslie::Move(Leslie::PieceType::PAWN, Leslie::Square::E5, Leslie::Square::D6),
		Leslie::Move(Leslie::PieceType::PAWN, Leslie::Square::E5, Leslie::Square::F6),
		Leslie::Move(Leslie::PieceType::PAWN, Leslie::Square::H6, Leslie::Square::H7),

	};
	EXPECT_EQ(expected.size(), actual.size());
	for (Leslie::Move move : actual)
		expected.erase(move);
	EXPECT_TRUE(expected.empty());
}
