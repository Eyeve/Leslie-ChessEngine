#ifndef LESLIE_PIECES_CONTAINER_H_
#define LESLIE_PIECES_CONTAINER_H_

#include <array>

#include "utility.h"

namespace Leslie {

class PiecesContainer {
 public:
  PiecesContainer();

  BitboardType GetBitboard(Piece piece) const;
  BitboardType GetBlockers(Color color) const;

  void SetBitboard(Piece piece, BitboardType value);

  Piece WhatPieceOnSquare(BitboardType square) const;

 private:
  std::array<std::array<BitboardType, 8>, 2> pieces_data_;
};

}  // namespace Leslie

#endif
