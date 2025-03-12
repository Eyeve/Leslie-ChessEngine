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

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
