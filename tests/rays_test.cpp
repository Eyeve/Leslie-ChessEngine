#include "magic.h"
#include "position.h"
#include <unordered_set>

#include <gtest/gtest.h>

class RaysFixture : public ::testing::Test {
  protected:
	void SetUp() override {

	}
};
int main() {
	Leslie::init_masks();
	Leslie::init_magic();
}

TEST_F(RaysFixture, up)
{
	int bit = 27;
	Leslie::bitboard expected = 0x808080800000000ull;
	Leslie::bitboard actual = Leslie::Rays[Leslie::Direction::UP][bit];
	EXPECT_EQ(expected, actual);
}



