#ifndef LESLIE_POSITION_H_
#define LESLIE_POSITION_H_

#include <string>
#include <vector>

#include "move.h"
#include "pieces_container.h"
#include "position.h"
#include "utility.h"

namespace Leslie {

class Position {
 public:
  explicit Position(const std::string& fen);

  Color GetMyColor() const;
  Color GetOpColor() const;

  Piece WhatPieceOnSquare(BitboardType sq) const;

  void AddPossibleMoves(std::vector<Move>& vec) const;

  Position MakeMoves(const std::vector<Move>& moves) const;
  Position MakeMove(Move move) const;

  int16_t GetSimpleEstimation() const;

 private:
  using MovesGetter = BitboardType (Position::*)(BitboardType, BitboardType) const;

  PiecesContainer pieces_;

  BitboardType en_passant_;
  CounterType rule_50_;
  CounterType moves_;

  Color current_;
  bool w_king_castle;
  bool w_queen_castle;
  bool b_king_castle;
  bool b_queen_castle;

  bool IsMoveMadeValid() const;

  void MakeMoveInPlace(Move move);

  void AddPieceMoves(MovesGetter getter, PieceType type, std::vector<Move>& moves) const;

  const BitboardType& GetMyBitboard(PieceType type) const;
  const BitboardType& GetOpBitboard(PieceType type) const;
  BitboardType& GetMyBitboard(PieceType type);
  BitboardType& GetOpBitboard(PieceType type);

  BitboardType GetKingsMoves(BitboardType sqs, BitboardType blockers) const;
  BitboardType GetQueensMoves(BitboardType sqs, BitboardType blockers) const;
  BitboardType GetRooksMoves(BitboardType sqs, BitboardType blockers) const;
  BitboardType GetBishopsMoves(BitboardType sqs, BitboardType blockers) const;
  BitboardType GetKnightsMoves(BitboardType sqs, BitboardType blockers) const;
  BitboardType GetWhitePawnsMoves(BitboardType sqs, BitboardType blockers) const;
  BitboardType GetBlackPawnsMoves(BitboardType sqs, BitboardType blockers) const;

  BitboardType GetQueenMoves(BitboardType sq, BitboardType blockers) const;
  BitboardType GetRookMoves(BitboardType sq, BitboardType blockers) const;
  BitboardType GetBishopMoves(BitboardType sq, BitboardType blockers) const;

  BitboardType GetPieceMoves(MovesGetter getter, BitboardType sqs, BitboardType blockers) const;
};

}  // namespace Leslie

#endif
