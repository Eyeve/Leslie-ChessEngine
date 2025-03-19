#ifndef LESLIE_TESTS_H_
#define LESLIE_TESTS_H_

#include <move.h>

#include <string>
#include <vector>

namespace leslie::test {

struct PieceMoves {
  PieceType type;
  Square from;
  std::vector<Square> moves;
};

using MoveContainerType = std::vector<PieceMoves>;

void PositionTest(const std::string& fen, const MoveContainerType& res);

}  // namespace leslie::test

#endif
