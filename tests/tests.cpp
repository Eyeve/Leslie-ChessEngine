#include "tests.h"

#include <gtest/gtest.h>

#include <unordered_set>

#include "engine.h"

// TODO: ref

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
  leslie::BitboardType actual = engine.GetMasks().rook_masks[bit];
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
  leslie::BitboardType actual = engine.GetMasks().bishop_masks[bit];
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

namespace leslie::test {

void PositionTest(const std::string& fen, const MoveContainerType& res) {
  std::unordered_set<Move, MoveHash> expected;
  for (const auto& [type, from, moves] : res) {
    for (const auto to : moves) {
      expected.insert(Move(type, from, to));
    }
  }

  Position position(fen);
  std::vector<Move> actual;
  position.GetMoves(actual);

  EXPECT_EQ(expected.size(), actual.size());
  for (Move move : actual) expected.erase(move);
  EXPECT_TRUE(expected.empty());
}

}  // namespace leslie::test

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}