#include "position.h"

#include <immintrin.h>

#include <bit>
#include <sstream>
#include <utility>

#include "board.h"
#include "move_table.h"

namespace Leslie {

Position::Position(const std::string& fen)
    : en_passant_(),
      rule_50_(),
      moves_(),
      turn_(),
      w_king_castle(),
      w_queen_castle(),
      b_king_castle(),
      b_queen_castle() {
  // TODO: add checks for invalid input
  std::istringstream iss(fen);
  std::string board_part, turn_part, castling_part, en_passant_part;

  iss >> board_part >> turn_part >> castling_part >> en_passant_part >> rule_50_ >> moves_;

  BitboardType sq = Board::Start();
  for (const char c : board_part) {
    if (c == '/') continue;

    if (isdigit(c)) {
      sq = Board::Shift(sq, c - '0');
      continue;
    }

    const Piece piece = ConvertCharToPiece(c);
    const BitboardType bitboard = pieces_.GetBitboard(piece);
    pieces_.SetBitboard(piece, bitboard | sq);
    sq = Board::Next(sq);
  }

  turn_ = (turn_part == "w") ? WHITE : BLACK;

  w_king_castle = castling_part.find('K') != std::string::npos;
  w_queen_castle = castling_part.find('Q') != std::string::npos;
  b_king_castle = castling_part.find('k') != std::string::npos;
  b_queen_castle = castling_part.find('q') != std::string::npos;

  if (en_passant_part != "-") {
    const int file = 'h' - en_passant_part[0];
    const int rank = en_passant_part[1] - '1';
    en_passant_ = (1ull << (rank * 8 + file));
  }
  UpdateBlockers();
}

const PiecesContainer& Position::GetPieceContainer() const {
  return pieces_;
}

Color Position::GetMyColor() const { return turn_; }

Color Position::GetOpColor() const {
  return static_cast<Color>(turn_ ^ 0b1);
}

BitboardType Position::GetMyBitboard(const PieceType type) const {
  return pieces_.GetBitboard(Piece(type, GetMyColor()));
}

BitboardType Position::GetOpBitboard(const PieceType type) const {
  return pieces_.GetBitboard(Piece(type, GetOpColor()));
}

BitboardType Position::GetMyBlockers() const {
  return pieces_.GetBlockers(GetMyColor());
}

BitboardType Position::GetOpBlockers() const {
  return pieces_.GetBlockers(GetOpColor());
}

void Position::SetMyBitboard(const PieceType type, const BitboardType value) {
  return pieces_.SetBitboard(Piece(type, GetMyColor()), value);
}

void Position::SetOpBitboard(const PieceType type, const BitboardType value) {
  return pieces_.SetBitboard(Piece(type, GetOpColor()), value);
}

Piece Position::WhatPieceOnSquare(const BitboardType square) const {
  return pieces_.WhatPieceOnSquare(square);
}

// void Position::AddPossibleMoves(std::vector<Move>& vec) const {
//   AddPieceMoves(&Position::GetKingsMoves, KING, vec);
//   AddPieceMoves(&Position::GetQueenMoves, QUEEN, vec);
//   AddPieceMoves(&Position::GetRookMoves, ROOK, vec);
//   AddPieceMoves(&Position::GetBishopMoves, BISHOP, vec);
//   AddPieceMoves(&Position::GetKnightsMoves, KNIGHT, vec);
//   if (turn_ == WHITE)
//     AddPieceMoves(&Position::GetWhitePawnsMoves, PAWN, vec);
//   else
//     AddPieceMoves(&Position::GetBlackPawnsMoves, PAWN, vec);
// }

Position Position::MakeMove(const Move move) const {
  Position my_copy(*this);
  my_copy.MakeMoveInPlace(move);
  return my_copy;
}

bool Position::IsKingSafe(const Color color) const {
  // TODO: redo
  const BitboardType king_sq = GetOpBitboard(KING);
  const BitboardType blockers =
      GetMyBlockers() | GetOpBlockers();
  const MoveTable& mt = MoveTable::Instance();

  BitboardType moves = mt.GetKingsMoves(GetMyBitboard(KING), blockers);
  moves |= mt.GetQueensMoves(GetMyBitboard(QUEEN), blockers);
  moves |= mt.GetRooksMoves(GetMyBitboard(ROOK), blockers);
  moves |= mt.GetBishopsMoves(GetMyBitboard(BISHOP), blockers);
  moves |= mt.GetKnightsMoves(GetMyBitboard(KNIGHT), blockers);
  moves |= mt.GetPawnsMoves(GetMyBitboard(PAWN), blockers, en_passant_, GetMyColor());
  return static_cast<bool>(~moves & king_sq);
}

void Position::UpdateBlockers() {
  // TODO: can be optimized
  blockers_[WHITE] = pieces_.GetBlockers(WHITE);
  blockers_[BLACK] = pieces_.GetBlockers(BLACK);
}


void Position::MakeMoveInPlace(const Move move) {
  // TODO: pawn promoting implementation
  // TODO: en passant capture implementation
  const BitboardType bitboard = GetMyBitboard(move.type);
  SetMyBitboard(move.type, bitboard & ~move.from | move.to);

  const BitboardType mask = ~move.to;
  for (const PieceType type: kPieceTypes)
    SetOpBitboard(type, GetOpBitboard(type) & mask);

  ++moves_;
  turn_ = GetOpColor();
  UpdateBlockers();
}

// void Position::AddPieceMoves(const MovesGetter getter, const PieceType type,
//                              std::vector<Move>& moves) const {
//   // TODO: move general parts outside
//   const BitboardType my_blockers = pieces_.GetBlockers(GetMyColor());
//   const BitboardType op_blockers = pieces_.GetBlockers(GetOpColor());
//   const BitboardType valid_squares = ~my_blockers;
//   const BitboardType blockers = my_blockers | op_blockers;
//
//   BitboardType from = GetMyBitboard(type);
//   while (from) {
//     const BitboardType from_sq = 1ull << std::countr_zero(from);
//
//     BitboardType to = (this->*getter)(from_sq, blockers) & valid_squares;
//     while (to) {
//       const BitboardType to_sq = 1ull << std::countr_zero(to);
//       const Move move(type, from_sq, to_sq);
//
//       if (MakeMove(move).IsMoveMadeValid()) moves.push_back(move);
//       to ^= to_sq;
//     }
//     from ^= from_sq;
//   }
// }

// EstimationType Position::GetSimpleEstimation() const {
//   EstimationType score = 0;
//   for (const PieceType piece_type : kPieceTypes) {
//     const BitboardType my_bb = GetMyBitboard(piece_type);
//     const BitboardType op_bb = GetOpBitboard(piece_type);
//
//     // TODO: type convert redo
//     score += __builtin_popcountll(my_bb) * GetPieceCost(piece_type);
//     score -= __builtin_popcountll(op_bb) * GetPieceCost(piece_type);
//   }
//   return GetOpColor() == WHITE ? -score : score;
// }

}  // namespace Leslie
