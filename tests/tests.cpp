#include <gtest/gtest.h>

#include "board.h"

TEST(general, to_str)
{
	constexpr std::string expected = "00000000\n10100101\n11111111\n00000000\n11110000\n00000000\n00000000\n11111111\n";
	constexpr Leslie::bitboard row = 0b0000000010100101111111110000000011110000000000000000000011111111;
	EXPECT_EQ(expected, Leslie::to_str(row));
}

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
