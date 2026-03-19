#ifndef LESLIE_TESTS_H_
#define LESLIE_TESTS_H_

#include <string>
#include <vector>

#include "board.h"
#include "move.h"

namespace Leslie::Test {

struct PieceMoves {
  PieceType type;
  Square from;
  std::vector<Square> moves;
};

using MoveContainerType = std::vector<PieceMoves>;

void PositionTest(const std::string& fen, const MoveContainerType& res);
std::vector<Move> CollectMoves(const std::string& fen);
bool HasMove(const std::vector<Move>& moves, const Move& move);

}  // namespace Leslie::Test

#endif
