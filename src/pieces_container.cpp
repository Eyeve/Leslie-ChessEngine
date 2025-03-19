#include "pieces_container.h"

#include <utility>

namespace leslie {

PiecesContainer::PiecesContainer() : pieces_data_() {}

BitboardType& PiecesContainer::GetBitboard(const Piece piece) {
  return pieces_data_[std::to_underlying(piece.color)]
                     [std::to_underlying(piece.type)];
}

const BitboardType& PiecesContainer::GetBitboard(const Piece piece) const {
  return pieces_data_[std::to_underlying(piece.color)]
                     [std::to_underlying(piece.type)];
}

BitboardType PiecesContainer::GetBlockers(const Color color) const {
  std::array colored_pieces = pieces_data_[std::to_underlying(color)];
  return colored_pieces[std::to_underlying(PieceType::kKing)] |
         colored_pieces[std::to_underlying(PieceType::kQueen)] |
         colored_pieces[std::to_underlying(PieceType::kRook)] |
         colored_pieces[std::to_underlying(PieceType::kBishop)] |
         colored_pieces[std::to_underlying(PieceType::kKnight)] |
         colored_pieces[std::to_underlying(PieceType::kPawn)];
}

Piece PiecesContainer::WhatPieceOnSquare(const BitboardType square) const {
  for (const auto color : kColors) {
    for (const auto type : kPieceTypes) {
      const Piece piece(type, color);
      if (GetBitboard(piece) & square)
        return piece;
    }
  }
  return Piece(PieceType::kNone, Color::kWhite);
}

}  // namespace leslie
