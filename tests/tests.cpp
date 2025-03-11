#include "board.h"
#include <unordered_set>

#include <gtest/gtest.h>


TEST(general, to_str)
{
	std::string expected = "00000000\n10100101\n11111111\n00000000\n11110000\n00000000\n00000000\n11111111\n";
	Leslie::bitboard row = 0b0000000010100101111111110000000011110000000000000000000011111111;
	EXPECT_EQ(expected, Leslie::to_str(row));
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

TEST(board, board_ctr)
{
	Leslie::Board board("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1");
	std::string expected = "rnbqkbnr\npppppppp\n........\n........\n....P...\n........\nPPPP.PPP\nRNBQKBNR\n";
	EXPECT_EQ(board.to_str(), expected);
}

TEST(knight_moves, test1)
{
	Leslie::Board board("N7/8/8/8/8/8/8/8 w - - 0 1");
	std::vector< Leslie::Move > moves;
	board.get_moves(moves);
	std::unordered_set< Leslie::Move, Leslie::MoveHash > expected = {
		Leslie::Move(Leslie::PieceType::KNIGHT, Leslie::Color::WHITE, Leslie::FileA & Leslie::Rank8, Leslie::FileC & Leslie::Rank7, Leslie::PieceType::NONE),
		Leslie::Move(Leslie::PieceType::KNIGHT, Leslie::Color::WHITE, Leslie::FileA & Leslie::Rank8, Leslie::FileB & Leslie::Rank6, Leslie::PieceType::NONE),
	};
	for (Leslie::Move move : moves)
		expected.erase(move);
	EXPECT_TRUE(expected.empty());
}

TEST(king_moves, test1)
{
	Leslie::Board board("K7/8/8/8/8/8/8/8 w ---- - 0 1");
	std::vector< Leslie::Move > moves;
	board.get_moves(moves);
	std::unordered_set< Leslie::Move, Leslie::MoveHash > expected = {
		Leslie::Move(Leslie::PieceType::KING, Leslie::Color::WHITE, Leslie::FileA & Leslie::Rank8, Leslie::FileB & Leslie::Rank8, Leslie::PieceType::NONE),
		Leslie::Move(Leslie::PieceType::KING, Leslie::Color::WHITE, Leslie::FileA & Leslie::Rank8, Leslie::FileB & Leslie::Rank7, Leslie::PieceType::NONE),
		Leslie::Move(Leslie::PieceType::KING, Leslie::Color::WHITE, Leslie::FileA & Leslie::Rank8, Leslie::FileA & Leslie::Rank7, Leslie::PieceType::NONE)
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
