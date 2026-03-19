#include "pieces_container.h"

#include <utility>

namespace Leslie {

PiecesContainer::PiecesContainer() : pieces_data_() {}

BitboardType PiecesContainer::GetBitboard(const Piece piece) const {
  return pieces_data_[piece.color][piece.type];
}

BitboardType PiecesContainer::GetBlockers(const Color color) const {
  const auto& colored_pieces = pieces_data_[color];
  return colored_pieces[KING] | colored_pieces[QUEEN] | colored_pieces[ROOK] |
         colored_pieces[BISHOP] | colored_pieces[KNIGHT] | colored_pieces[PAWN];
}

void PiecesContainer::SetBitboard(const Piece piece, const BitboardType value) {
  pieces_data_[piece.color][piece.type] = value;
}

Piece PiecesContainer::WhatPieceOnSquare(const BitboardType square) const {
  for (const auto color : kColors) {
    for (const auto type : kPieceTypes) {
      const Piece piece(type, color);
      if (GetBitboard(piece) & square) return piece;
    }
  }
  return Piece(NONE_PIECE_TYPE, WHITE);
}

}  // namespace Leslie
