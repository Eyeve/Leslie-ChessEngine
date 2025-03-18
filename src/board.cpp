#include "board.h"

#include <bit>

namespace leslie {

SizeType Board::GetStrBoardSize() { return str_board_size_; }

BitboardType Board::Start() {
  return static_cast<BitboardType>(Square::kA8);
}

BitboardType Board::Next(const BitboardType sq) { return sq >> 1; }

BitboardType Board::NextRank(const BitboardType sq) {
  return sq >> (std::countr_zero(sq) % 8 + 1);
}

BitboardType Board::Shift(const BitboardType sq, const int off) {
  return sq >> off;
}

bool Board::IsEnd(const BitboardType sq) { return sq > 0ull; }

bool Board::IsEndLine(BitboardType sq) {
  return std::countr_zero(sq) % 8 == 0;
}

}  // namespace leslie
