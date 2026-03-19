#ifndef LESLIE_TESTS_H_
#define LESLIE_TESTS_H_

#include <string>
#include <vector>

#include "board.h"

namespace Leslie::Test {

struct PieceMoves {
  PieceType type;
  Square from;
  std::vector<Square> moves;
};

using MoveContainerType = std::vector<PieceMoves>;

void PositionTest(const std::string& fen, const MoveContainerType& res);

}  // namespace Leslie::Test

#endif
