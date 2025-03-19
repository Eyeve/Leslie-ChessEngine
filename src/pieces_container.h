#ifndef LESLIE_PIECES_CONTAINER_H_
#define LESLIE_PIECES_CONTAINER_H_

#include <array>

#include "types.h"

namespace leslie {

class PiecesContainer {
 public:
  PiecesContainer();

  BitboardType& GetBitboard(Piece piece);
  const BitboardType& GetBitboard(Piece piece) const;
  BitboardType GetBlockers(Color color) const;

  Piece WhatPieceOnSquare(BitboardType square) const;

 private:
  std::array<std::array<BitboardType, 8>, 2> pieces_data_;
};

}  // namespace leslie

#endif
