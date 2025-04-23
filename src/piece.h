#ifndef LESLIE_PIECE_H_
#define LESLIE_PIECE_H_

#include <array>

namespace Leslie {

enum Color : bool {
  WHITE,
  BLACK,
};

constexpr std::array kColors{WHITE, BLACK};

enum PieceType {
  NONE_PIECE_TYPE = -1,
  KING,
  PAWN,
  KNIGHT,
  BISHOP,
  ROOK,
  QUEEN,
};

constexpr std::array<uint8_t, 6> kPiecesCost = {255, 10, 30, 31, 50, 90};

constexpr std::array kPieceTypes = {KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN};

struct Piece {
  PieceType type;
  Color color;
};

}  // namespace Leslie

#endif
