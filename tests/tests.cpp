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
	std::vector< Leslie::Move > moves;
	position.get_moves(moves);
	std::unordered_set< Leslie::Move, Leslie::MoveHash > expected = {
		Leslie::Move(Leslie::PieceType::KNIGHT, Leslie::FileA & Leslie::Rank8, Leslie::FileC & Leslie::Rank7),
		Leslie::Move(Leslie::PieceType::KNIGHT, Leslie::FileA & Leslie::Rank8, Leslie::FileB & Leslie::Rank6),
	};
	for (Leslie::Move move : moves)
		expected.erase(move);
	EXPECT_TRUE(expected.empty());
}

TEST(king_moves, test1)
{
	Leslie::Position position("K7/8/8/8/8/8/8/8 w ---- - 0 1");
	std::vector< Leslie::Move > moves;
	position.get_moves(moves);
	std::unordered_set< Leslie::Move, Leslie::MoveHash > expected = {
		Leslie::Move(Leslie::PieceType::KING, Leslie::FileA & Leslie::Rank8, Leslie::FileB & Leslie::Rank8),
		Leslie::Move(Leslie::PieceType::KING, Leslie::FileA & Leslie::Rank8, Leslie::FileB & Leslie::Rank7),
		Leslie::Move(Leslie::PieceType::KING, Leslie::FileA & Leslie::Rank8, Leslie::FileA & Leslie::Rank7),
	};
	for (Leslie::Move move : moves)
		expected.erase(move);
	EXPECT_TRUE(expected.empty());
}

TEST(pawn_moves, test1)
{
	Leslie::Position position("8/p7/1P5N/8/8/8/8/8 b - - 0 1");
	std::vector< Leslie::Move > moves;
	position.get_moves(moves);
	std::unordered_set< Leslie::Move, Leslie::MoveHash > expected = {
		Leslie::Move(Leslie::PieceType::PAWN, Leslie::FileA & Leslie::Rank7, Leslie::FileA & Leslie::Rank6),
		Leslie::Move(Leslie::PieceType::PAWN, Leslie::FileA & Leslie::Rank7, Leslie::FileB & Leslie::Rank6),
		Leslie::Move(Leslie::PieceType::PAWN, Leslie::FileA & Leslie::Rank7, Leslie::FileA & Leslie::Rank5),
	};
	for (Leslie::Move move : moves)
		expected.erase(move);
	EXPECT_TRUE(expected.empty());
}

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
