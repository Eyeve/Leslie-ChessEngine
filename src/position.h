#ifndef LESLIE_POSITION_H_
#define LESLIE_POSITION_H_

#include <string>
#include <vector>

#include "move.h"
#include "pieces_container.h"
#include "position.h"
#include "types.h"

namespace leslie {

class Position {
 public:
  explicit Position(const std::string& fen);
  Position(const Position& other) = default;

  Color GetMyColor() const;
  Color GetOpColor() const;

  Piece WhatPieceOnSquare(BitboardType sq) const;

  void AddPossibleMoves(std::vector<Move>& vec) const;
  Position MakeMoves(const std::vector<Move>& moves) const;
  Position MakeMove(const Move& move) const;

 private:
  using MovesGetter = BitboardType (Position::*)(BitboardType,
                                                 BitboardType) const;

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

  void MakeMoveInPlace(const Move& move);

  void AddPieceMoves(MovesGetter getter, PieceType type,
                     std::vector<Move>& moves) const;

  BitboardType GetMyBitboard(PieceType type) const;
  BitboardType GetOpBitboard(PieceType type) const;

  BitboardType GetKingsMoves(BitboardType sqs, BitboardType blockers) const;
  BitboardType GetQueensMoves(BitboardType sq, BitboardType blockers) const;
  BitboardType GetRooksMoves(BitboardType sq, BitboardType blockers) const;
  BitboardType GetBishopsMoves(BitboardType sq, BitboardType blockers) const;
  BitboardType GetKnightsMoves(BitboardType sqs, BitboardType blockers) const;
  BitboardType GetWhitePawnsMoves(BitboardType sqs,
                                  BitboardType blockers) const;
  BitboardType GetBlackPawnsMoves(BitboardType sqs,
                                  BitboardType blockers) const;

  BitboardType GetQueenMoves(BitboardType sqs, BitboardType blockers) const;
  BitboardType GetRookMoves(BitboardType sqs, BitboardType blockers) const;
  BitboardType GetBishopMoves(BitboardType sqs, BitboardType blockers) const;

  BitboardType GetPieceMoves(MovesGetter getter, BitboardType sqs,
                             BitboardType blockers) const;
};

}  // namespace leslie

#endif
