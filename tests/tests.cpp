#include <engine.h>
#include <gtest/gtest.h>

#include <unordered_set>

#include "magic.h"
#include "position.h"

#define MERGE_ROWS(a, b, c, d, e, f, g, h) \
  a "\n" b "\n" c "\n" d "\n" e "\n" f "\n" g "\n" h "\n"

TEST(general, to_str) {
  std::string expected =
      MERGE_ROWS("00000000", "10100101", "11111111", "00000000", "11110000",
                 "00000000", "00000000", "11111111");
  leslie::BitboardType row =
      0b0000000010100101111111110000000011110000000000000000000011111111;
  std::string actual = leslie::ToStr(row);
  EXPECT_STREQ(expected.c_str(), actual.c_str());
}

TEST(general, rank5) {
  leslie::BitboardType expected = 0x000000FF00000000ull;
  EXPECT_EQ(expected, leslie::kRank5);
}

TEST(general, fileF) {
  leslie::BitboardType expected = 0x0404040404040404ull;
  EXPECT_EQ(expected, leslie::kFileF);
}

TEST(general, rook_mask) {
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
  leslie::Engine& engine = leslie::Engine::Instance();
  leslie::BitboardType expected = 0x8080808F7080808ull;
  leslie::BitboardType actual = engine.GetMasks().RookMasks[bit];
  EXPECT_EQ(expected, actual);
}

TEST(general, bishop_mask) {
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
  leslie::Engine& engine = leslie::Engine::Instance();
  leslie::BitboardType expected = 0x8041221400142241ull;
  leslie::BitboardType actual = engine.GetMasks().BishopMasks[bit];
  EXPECT_EQ(expected, actual);
}

TEST(position, position_ctr) {
  leslie::Position position(
      "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1");
  std::string expected =
      MERGE_ROWS("rnbqkbnr", "pppppppp", "........", "........", "....P...",
                 "........", "PPPP.PPP", "RNBQKBNR");
  std::string actual = static_cast<std::string>(position);
  EXPECT_STREQ(expected.c_str(), actual.c_str());
}

TEST(rays, up) {
  /*
   * 00001000
   * 00001000
   * 00001000
   * 00001000
   * 00000000
   * 00000000
   * 00000000
   * 00000000
   */

  // int bit = 27;
  // leslie::Engine& engine = leslie::Engine::Instance();
  // leslie::BitboardType expected = 0x808080800000000ull;
  // leslie::BitboardType actual = engine.GetMasks()[leslie::Direction::kUp][bit];
  // EXPECT_EQ(expected, actual);
}

TEST(rays, right) {
  /*
   * 00000000
   * 00000000
   * 00000000
   * 00000000
   * 00000111
   * 00000000
   * 00000000
   * 00000000
   */

  // int bit = 27;
  // leslie::BitboardType expected = 0x7000000ull;
  // leslie::BitboardType actual = leslie::Rays[leslie::Direction::kRight][bit];
  // EXPECT_EQ(expected, actual);
}

TEST(rays, down) {
  /*
   * 00000000
   * 00000000
   * 00000000
   * 00000000
   * 00000000
   * 00001000
   * 00001000
   * 00001000
   */

  // int bit = 27;
  // leslie::BitboardType expected = 0x80808ull;
  // leslie::BitboardType actual = leslie::Rays[leslie::Direction::kDown][bit];
  // EXPECT_EQ(expected, actual);
}

TEST(rays, left) {
  /*
   * 00000000
   * 00000000
   * 00000000
   * 00000000
   * 11110000
   * 00000000
   * 00000000
   * 00000000
   */

  // int bit = 27;
  // leslie::BitboardType expected = 0xF0000000ull;
  // leslie::BitboardType actual = leslie::Rays[leslie::Direction::kLeft][bit];
  // EXPECT_EQ(expected, actual);
}

TEST(rays, up_right) {
  /*
   * 00000000
   * 00000001
   * 00000010
   * 00000100
   * 00000000
   * 00000000
   * 00000000
   * 00000000
   */

  // int bit = 27;
  // leslie::BitboardType expected = 0x1020400000000ull;
  // leslie::BitboardType actual = leslie::Rays[leslie::Direction::kUpRight][bit];
  // EXPECT_EQ(expected, actual);
}

TEST(rays, up_left) {
  /*
   * 10000000
   * 01000000
   * 00100000
   * 00010000
   * 00000000
   * 00000000
   * 00000000
   * 00000000
   */

  // int bit = 27;
  // leslie::BitboardType expected = 0x8040201000000000ull;
  // leslie::BitboardType actual = leslie::Rays[leslie::Direction::kUpLeft][bit];
  // EXPECT_EQ(expected, actual);
}

TEST(rays, down_right) {
  /*
   * 00000000
   * 00000000
   * 00000000
   * 00000000
   * 00000000
   * 00000100
   * 00000010
   * 00000001
   */

  // int bit = 27;
  // leslie::BitboardType expected = 0x40201ull;
  // leslie::BitboardType actual = leslie::Rays[leslie::Direction::kDownRight][bit];
  // EXPECT_EQ(expected, actual);
}

TEST(rays, down_left) {
  /*
   * 00000000
   * 00000000
   * 00000000
   * 00000000
   * 00000000
   * 00010000
   * 00100000
   * 01000000
   */

  // int bit = 27;
  // leslie::BitboardType expected = 0x102040ull;
  // leslie::BitboardType actual = leslie::Rays[leslie::Direction::kDownLeft][bit];
  // EXPECT_EQ(expected, actual);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}