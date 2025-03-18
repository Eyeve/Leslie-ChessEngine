#ifndef LESLIE_MYBOARD_H_
#define LESLIE_MYBOARD_H_

#include "types.h"

namespace leslie {

class Board {
 public:
  Board() = delete;
  Board(const Board&) = delete;
  Board& operator=(const Board&) = delete;
  ~Board() = delete;

  static SizeType GetStrBoardSize();
  static BitboardType Start();
  static BitboardType Next(BitboardType sq);
  static BitboardType NextRank(BitboardType sq);
  static BitboardType Shift(BitboardType sq, int off);
  static bool IsEnd(BitboardType sq);
  static bool IsEndLine(BitboardType sq);

 private:
  static constexpr SizeType str_board_size_ = 64 + 8 + 1;
};

}  // namespace leslie

#endif
