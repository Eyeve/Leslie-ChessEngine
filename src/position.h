#ifndef LESLIE_BOARD_H_
#define LESLIE_BOARD_H_

#include <string>
#include <vector>

#include "move.h"
#include "pieces_container.h"
#include "position.h"
#include "types.h"

namespace leslie {

std::string ToStr(BitboardType bitboard);

class Position {
 public:
  explicit Position(const std::string& fen);
  Position(const Position& pos) = default;

  explicit operator std::string() const;

  Position MakeMoves(Move* moves, SizeType size) const;
  void GetMoves(std::vector<Move>& vec) const;

 private:
  PiecesContainer pieces_;

  BitboardType en_passant_;
  CounterType rule_50_;
  CounterType moves_;

  Color turn_;
  bool w_king_castle;
  bool w_queen_castle;
  bool b_king_castle;
  bool b_queen_castle;

  static Piece CharToPiece(char c);
  static char PieceToChar(Piece piece);

  Color GetOpponent() const;

  void AddMoves(void (Position::*adder)(BitboardType, Color, std::vector<Move>&)
                    const,
                BitboardType pieces, Color color, std::vector<Move>& vec) const;
  void AddPieceMoves(BitboardType from, BitboardType to, PieceType type,
                     std::vector<Move>& vec) const;

  void AddKingMoves(BitboardType position, Color color,
                    std::vector<Move>& vec) const;
  void AddQueenMoves(BitboardType position, Color color,
                     std::vector<Move>& vec) const;
  void AddRookMoves(BitboardType position, Color color,
                    std::vector<Move>& vec) const;
  void AddBishopMoves(BitboardType position, Color color,
                      std::vector<Move>& vec) const;
  void AddKnightMoves(BitboardType position, Color color,
                      std::vector<Move>& vec) const;
  void AddPawnMoves(BitboardType position, Color color,
                    std::vector<Move>& vec) const;

  void AddWhitePawnMoves(BitboardType position, std::vector<Move>& vec) const;
  void AddBlackPawnMoves(BitboardType position, std::vector<Move>& vec) const;
};
}  // namespace leslie

#endif
