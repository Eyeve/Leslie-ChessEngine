#include "position.h"

#include <cctype>
#include <immintrin.h>

#include <bit>
#include <sstream>
#include <utility>

#include "board.h"
#include "move_table.h"

namespace Leslie {

Position::Position(const std::string& fen)
    : en_passant_(0ull),
      rule_50_(0),
      moves_(0),
      turn_(WHITE),
      w_king_castle(false),
      w_queen_castle(false),
      b_king_castle(false),
      b_queen_castle(false) {
  std::istringstream iss(fen);
  std::string board_part;
  std::string turn_part;
  std::string castling_part;
  std::string en_passant_part;
  int rule_50 = 0;
  int moves = 0;

  if (!(iss >> board_part >> turn_part >> castling_part >> en_passant_part >> rule_50 >> moves)) {
    board_part = Board::kStartFen;
    std::istringstream fallback(Board::kStartFen);
    fallback >> board_part >> turn_part >> castling_part >> en_passant_part >> rule_50 >> moves;
  }

  rule_50_ = static_cast<CounterType>(rule_50);
  moves_ = static_cast<CounterType>(moves);

  BitboardType sq = Board::Start();
  for (const char c : board_part) {
    if (c == '/') {
      continue;
    }

    if (std::isdigit(static_cast<unsigned char>(c))) {
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

  en_passant_ = 0ull;
  if (en_passant_part.size() == 2 && en_passant_part != "-") {
    const int file = 'h' - en_passant_part[0];
    const int rank = en_passant_part[1] - '1';
    if (file >= 0 && file < 8 && rank >= 0 && rank < 8) {
      en_passant_ = 1ull << (rank * 8 + file);
    }
  }

  UpdateBlockers();
}

const PiecesContainer& Position::GetPieceContainer() const { return pieces_; }

Color Position::GetMyColor() const { return turn_; }

Color Position::GetOpColor() const { return static_cast<Color>(turn_ ^ 0b1); }

BitboardType Position::GetMyBitboard(const PieceType type) const {
  return pieces_.GetBitboard(Piece(type, GetMyColor()));
}

BitboardType Position::GetOpBitboard(const PieceType type) const {
  return pieces_.GetBitboard(Piece(type, GetOpColor()));
}

BitboardType Position::GetMyBlockers() const { return blockers_[GetMyColor()]; }

BitboardType Position::GetOpBlockers() const { return blockers_[GetOpColor()]; }

BitboardType Position::GetEnPassant() const { return en_passant_; }

bool Position::CanCastleKingSide(const Color color) const {
  return color == WHITE ? w_king_castle : b_king_castle;
}

bool Position::CanCastleQueenSide(const Color color) const {
  return color == WHITE ? w_queen_castle : b_queen_castle;
}

void Position::SetMyBitboard(const PieceType type, const BitboardType value) {
  pieces_.SetBitboard(Piece(type, GetMyColor()), value);
}

void Position::SetOpBitboard(const PieceType type, const BitboardType value) {
  pieces_.SetBitboard(Piece(type, GetOpColor()), value);
}

Piece Position::WhatPieceOnSquare(const BitboardType square) const {
  return pieces_.WhatPieceOnSquare(square);
}

void Position::AddPossibleMoves(std::vector<Move>& out) const {
  std::vector<OrderManager::Data> data;
  MoveTable::Instance().RefreshValidMoves(*this, data);

  out.clear();
  out.reserve(data.size());
  for (const auto& item : data) {
    out.push_back(item.move);
  }
}

Position Position::MakeMove(const Move move) const {
  Position my_copy(*this);
  my_copy.MakeMoveInPlace(move);
  return my_copy;
}

bool Position::IsKingSafe(const Color color) const {
  const Color op_color = static_cast<Color>(color ^ 0b1);
  const BitboardType king_sq = pieces_.GetBitboard(Piece(KING, color));
  if (king_sq == 0ull) {
    return false;
  }
  return !MoveTable::Instance().IsSquareAttacked(*this, king_sq, op_color);
}

EstimationType Position::GetSimpleEstimation() const {
  EstimationType score = 0;
  for (const PieceType piece_type : kPieceTypes) {
    const BitboardType my_bb = GetMyBitboard(piece_type);
    const BitboardType op_bb = GetOpBitboard(piece_type);
    score += static_cast<EstimationType>(std::popcount(my_bb)) * GetPieceCost(piece_type);
    score -= static_cast<EstimationType>(std::popcount(op_bb)) * GetPieceCost(piece_type);
  }
  return GetOpColor() == WHITE ? -score : score;
}

void Position::UpdateBlockers() {
  blockers_[WHITE] = pieces_.GetBlockers(WHITE);
  blockers_[BLACK] = pieces_.GetBlockers(BLACK);
}

void Position::MakeMoveInPlace(const Move move) {
  const BitboardType from = move.from;
  const BitboardType to = move.to;

  SetMyBitboard(move.type, GetMyBitboard(move.type) & ~from);

  if (move.promotion != NONE_PIECE_TYPE && move.type == PAWN) {
    SetMyBitboard(move.promotion, GetMyBitboard(move.promotion) | to);
  } else {
    SetMyBitboard(move.type, GetMyBitboard(move.type) | to);
  }

  if (move.en_passant && move.type == PAWN) {
    const BitboardType captured = (turn_ == WHITE) ? (to >> 8) : (to << 8);
    SetOpBitboard(PAWN, GetOpBitboard(PAWN) & ~captured);
  } else {
    const BitboardType clear_mask = ~to;
    for (const PieceType type : kPieceTypes) {
      SetOpBitboard(type, GetOpBitboard(type) & clear_mask);
    }
  }

  if (move.castling && move.type == KING) {
    if ((from >> 2) == to) {
      const BitboardType rook_from = from >> 3;
      const BitboardType rook_to = from >> 1;
      SetMyBitboard(ROOK, (GetMyBitboard(ROOK) & ~rook_from) | rook_to);
    } else if ((from << 2) == to) {
      const BitboardType rook_from = from << 4;
      const BitboardType rook_to = from << 1;
      SetMyBitboard(ROOK, (GetMyBitboard(ROOK) & ~rook_from) | rook_to);
    }
  }

  if (move.type == KING) {
    if (turn_ == WHITE) {
      w_king_castle = false;
      w_queen_castle = false;
    } else {
      b_king_castle = false;
      b_queen_castle = false;
    }
  }

  if (move.type == ROOK) {
    if (turn_ == WHITE) {
      if (from == SQ_H1) w_king_castle = false;
      if (from == SQ_A1) w_queen_castle = false;
    } else {
      if (from == SQ_H8) b_king_castle = false;
      if (from == SQ_A8) b_queen_castle = false;
    }
  }

  if (move.capture == ROOK || move.en_passant) {
    if (to == SQ_H1) w_king_castle = false;
    if (to == SQ_A1) w_queen_castle = false;
    if (to == SQ_H8) b_king_castle = false;
    if (to == SQ_A8) b_queen_castle = false;
  }

  en_passant_ = 0ull;
  if (move.type == PAWN) {
    if ((from << 16) == to) {
      en_passant_ = from << 8;
    } else if ((from >> 16) == to) {
      en_passant_ = from >> 8;
    }
  }

  if (move.type == PAWN || move.capture != NONE_PIECE_TYPE || move.en_passant) {
    rule_50_ = 0;
  } else {
    ++rule_50_;
  }

  ++moves_;
  turn_ = GetOpColor();
  UpdateBlockers();
}

}  // namespace Leslie
