#ifndef LESLIE_PIECES_CONTAINER_H_
#define LESLIE_PIECES_CONTAINER_H_

#include <array>

#include "move.h"
#include "types.h"

namespace Leslie {

class PiecesContainer {
 public:
  PiecesContainer();
  PiecesContainer(const PiecesContainer& other, const Move& move, Color color);

  BitboardType& GetBitboard(Piece piece);
  const BitboardType& GetBitboard(Piece piece) const;
  BitboardType GetBlockers(Color color) const;

  Piece WhatPieceOnSquare(BitboardType square) const;

 private:
  std::array<std::array<BitboardType, 8>, 2> pieces_data_;
};

}  // namespace Leslie

#endif
