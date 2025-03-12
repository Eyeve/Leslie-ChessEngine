#include "position.h"
#include <unordered_set>

#include <gtest/gtest.h>

#define MERGE_ROWS(a, b, c, d, e, f, g, h) a "\n" b "\n" c "\n" d "\n" e "\n" f "\n" g "\n" h "\n"


TEST(general, to_str)
{
	std::string expected = MERGE_ROWS(
		"00000000",
		"10100101",
		"11111111",
		"00000000",
		"11110000",
		"00000000",
		"00000000",
		"11111111"
	);
	Leslie::bitboard row = 0b0000000010100101111111110000000011110000000000000000000011111111;
	std::string actual = Leslie::to_str(row);
	EXPECT_STREQ(expected.c_str(), actual.c_str());
}

TEST(general, rank5)
{
	Leslie::bitboard expected = 0x000000FF00000000;
	EXPECT_EQ(expected, Leslie::Rank5);
}

TEST(general, fileF)
{
	Leslie::bitboard expected = 0x0404040404040404;
	EXPECT_EQ(expected, Leslie::FileF);
}

TEST(position, position_ctr)
{
	Leslie::Position position("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1");
	std::string expected = MERGE_ROWS(
		"rnbqkbnr",
		"pppppppp",
		"........",
		"........",
		"....P...",
		"........",
		"PPPP.PPP",
		"RNBQKBNR"
	);
	std::string actual = static_cast<std::string>(position);
	EXPECT_STREQ(expected.c_str(), actual.c_str());
}

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

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
