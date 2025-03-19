#ifndef LESLIE_BOARD_H_
#define LESLIE_BOARD_H_

#include "types.h"

namespace leslie {

class Board {
 public:
  Board() = delete;
  Board(const Board&) = delete;
  Board& operator=(const Board&) = delete;
  ~Board() = delete;

  static constexpr SizeType GetStrBoardSize();
  static constexpr BitboardType Start();
  static constexpr BitboardType Next(BitboardType sq);
  static constexpr BitboardType NextRank(BitboardType sq);
  static constexpr BitboardType Shift(BitboardType sq, int off);
  static constexpr bool IsEnd(BitboardType sq);
  static constexpr bool IsEndLine(BitboardType sq);

 private:
  static constexpr SizeType str_board_size_ = 64 + 8 + 1;
};

/*
 * BOARD DEFINITIONS
 */

constexpr SizeType Board::GetStrBoardSize() { return str_board_size_; }

constexpr BitboardType Board::Start() {
  return static_cast<BitboardType>(Square::kA8);
}

constexpr BitboardType Board::Next(const BitboardType sq) { return sq >> 1; }

constexpr BitboardType Board::NextRank(const BitboardType sq) {
  return sq >> (std::countr_zero(sq) % 8 + 1);
}

constexpr BitboardType Board::Shift(const BitboardType sq, const int off) {
  return sq >> off;
}

constexpr bool Board::IsEnd(const BitboardType sq) { return sq > 0ull; }

constexpr bool Board::IsEndLine(const BitboardType sq) {
  return std::countr_zero(sq) % 8 == 0;
}

}  // namespace leslie

#endif
