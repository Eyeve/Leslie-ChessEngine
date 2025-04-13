#include "pieces_container.h"

#include <utility>

namespace Leslie {

PiecesContainer::PiecesContainer() : pieces_data_() {}

BitboardType& PiecesContainer::GetBitboard(const Piece piece) {
  return pieces_data_[std::to_underlying(piece.color)][std::to_underlying(piece.type)];
}

const BitboardType& PiecesContainer::GetBitboard(const Piece piece) const {
  return pieces_data_[std::to_underlying(piece.color)][std::to_underlying(piece.type)];
}

BitboardType PiecesContainer::GetBlockers(const Color color) const {
  std::array colored_pieces = pieces_data_[std::to_underlying(color)];
  return colored_pieces[std::to_underlying(KING)] | colored_pieces[std::to_underlying(QUEEN)] |
         colored_pieces[std::to_underlying(ROOK)] | colored_pieces[std::to_underlying(BISHOP)] |
         colored_pieces[std::to_underlying(KNIGHT)] | colored_pieces[std::to_underlying(PAWN)];
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
