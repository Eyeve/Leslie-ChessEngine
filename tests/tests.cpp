#include <unordered_set>
#include <gtest/gtest.h>

#include "position.h"
#include "magic.h"

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
	Leslie::bitboard expected = 0x000000FF00000000ull;
	EXPECT_EQ(expected, Leslie::Rank5);
}

TEST(general, fileF)
{
	Leslie::bitboard expected = 0x0404040404040404ull;
	EXPECT_EQ(expected, Leslie::FileF);
}

TEST(general, rook_mask)
{
	/*
	 * 00001000
	 * 00001000
	 * 00001000
	 * 00001000
	 * 11110111
	 * 00001000
	 * 00001000
	 * 00001000
	 */

	int bit = 27;
	Leslie::bitboard expceted = 0x8080808F7080808ull;
	Leslie::bitboard actual = Leslie::RookMasks[bit];
	EXPECT_EQ(expceted, actual);
}

TEST(general, bishop_mask)
{
	/*
	 * 10000000
	 * 01000001
	 * 00100010
	 * 00010100
	 * 0000B000
	 * 00010100
	 * 00100010
	 * 01000001
	 */

	int bit = 27;
	Leslie::bitboard expceted = 0x8041221400142241ull;
	Leslie::bitboard actual = Leslie::BishopMasks[bit];
	EXPECT_EQ(expceted, actual);
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


int main(int argc, char* argv[])
{
	Leslie::init_masks();
	Leslie::init_magic();

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
