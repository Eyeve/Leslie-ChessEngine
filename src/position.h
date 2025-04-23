#ifndef LESLIE_POSITION_H_
#define LESLIE_POSITION_H_

#include <string>

#include "move.h"
#include "pieces_container.h"
#include "position.h"
#include "utility.h"

namespace Leslie {

class Position {
 public:
  explicit Position(const std::string& fen);

  const PiecesContainer& GetPieceContainer() const;
  Color GetMyColor() const;
  Color GetOpColor() const;
  BitboardType GetMyBitboard(PieceType type) const;
  BitboardType GetOpBitboard(PieceType type) const;
  BitboardType GetMyBlockers() const;
  BitboardType GetOpBlockers() const;

  void SetMyBitboard(PieceType type, BitboardType value);
  void SetOpBitboard(PieceType type, BitboardType value);

  Piece WhatPieceOnSquare(BitboardType square) const;

  Position MakeMove(Move move) const;
  bool IsKingSafe(Color color) const;

 private:
  PiecesContainer pieces_;
  std::array<BitboardType, 2> blockers_;

  BitboardType en_passant_;
  CounterType rule_50_;
  CounterType moves_;

  Color turn_;
  bool w_king_castle;
  bool w_queen_castle;
  bool b_king_castle;
  bool b_queen_castle;

  void UpdateBlockers();
  void MakeMoveInPlace(Move move);
};

}  // namespace Leslie

#endif
