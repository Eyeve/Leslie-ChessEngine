#ifndef LESLIE_BOARD_H_
#define LESLIE_BOARD_H_

#include "utility.h"

#define LESLIE_RANK(a, b, c, d, e, f, g, h, R) \
  a = FILE_A & R, b = a >> 1, c = a >> 2, d = a >> 3, e = a >> 4, f = a >> 5, g = a >> 6, h = a >> 7

namespace Leslie {

enum BoardMask : BitboardType {
  RANK_1 = 0xffull,
  RANK_2 = RANK_1 << (8 * 1),
  RANK_3 = RANK_1 << (8 * 2),
  RANK_4 = RANK_1 << (8 * 3),
  RANK_5 = RANK_1 << (8 * 4),
  RANK_6 = RANK_1 << (8 * 5),
  RANK_7 = RANK_1 << (8 * 6),
  RANK_8 = RANK_1 << (8 * 7),

  FILE_A = 0x8080808080808080ull,
  FILE_B = FILE_A >> 1,
  FILE_C = FILE_A >> 2,
  FILE_D = FILE_A >> 3,
  FILE_E = FILE_A >> 4,
  FILE_F = FILE_A >> 5,
  FILE_G = FILE_A >> 6,
  FILE_H = FILE_A >> 7,
};

enum Square : BitboardType {
  LESLIE_RANK(SQ_A8, SQ_B8, SQ_C8, SQ_D8, SQ_E8, SQ_F8, SQ_G8, SQ_H8, RANK_8),
  LESLIE_RANK(SQ_A7, SQ_B7, SQ_C7, SQ_D7, SQ_E7, SQ_F7, SQ_G7, SQ_H7, RANK_7),
  LESLIE_RANK(SQ_A6, SQ_B6, SQ_C6, SQ_D6, SQ_E6, SQ_F6, SQ_G6, SQ_H6, RANK_6),
  LESLIE_RANK(SQ_A5, SQ_B5, SQ_C5, SQ_D5, SQ_E5, SQ_F5, SQ_G5, SQ_H5, RANK_5),
  LESLIE_RANK(SQ_A4, SQ_B4, SQ_C4, SQ_D4, SQ_E4, SQ_F4, SQ_G4, SQ_H4, RANK_4),
  LESLIE_RANK(SQ_A3, SQ_B3, SQ_C3, SQ_D3, SQ_E3, SQ_F3, SQ_G3, SQ_H3, RANK_3),
  LESLIE_RANK(SQ_A2, SQ_B2, SQ_C2, SQ_D2, SQ_E2, SQ_F2, SQ_G2, SQ_H2, RANK_2),
  LESLIE_RANK(SQ_A1, SQ_B1, SQ_C1, SQ_D1, SQ_E1, SQ_F1, SQ_G1, SQ_H1, RANK_1),
};

/*
 * Full static class responsible for interacting with the game 8x8 chess board
 */
class Board {
 public:
  static constexpr auto kStartFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  static constexpr SizeType kSideLength = 8;
  static constexpr SizeType kSize = kSideLength * kSideLength;
  static constexpr SizeType kStrFormatSize = kSize + kSideLength + 1;

  Board() = delete;
  ~Board() = delete;

  static constexpr BitboardType Start();
  static constexpr BitboardType Next(BitboardType sq);
  static constexpr BitboardType NextRank(BitboardType sq);
  static constexpr BitboardType Shift(BitboardType sq, int off);
  static constexpr bool IsEnd(BitboardType sq);
  static constexpr bool IsEndLine(BitboardType sq);
};

/*
 * BOARD DEFINITIONS
 */

constexpr BitboardType Board::Start() { return SQ_A8; }

constexpr BitboardType Board::Next(const BitboardType sq) { return sq >> 1; }

constexpr BitboardType Board::NextRank(const BitboardType sq) {
  return sq >> (std::countr_zero(sq) % kSideLength + 1);
}

constexpr BitboardType Board::Shift(const BitboardType sq, const int off) { return sq >> off; }

constexpr bool Board::IsEnd(const BitboardType sq) { return sq > 0ull; }

constexpr bool Board::IsEndLine(const BitboardType sq) {
  return std::countr_zero(sq) % kSideLength == 0;
}

}  // namespace Leslie

#endif
