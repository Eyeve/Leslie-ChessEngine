#include <gtest/gtest.h>

#include "board.h"

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

TEST(general, knight)
{

}

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}