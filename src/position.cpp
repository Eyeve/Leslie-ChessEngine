#include "position.h"

#include <engine.h>
#include <immintrin.h>

#include <bit>
#include <sstream>
#include <utility>

#include "board.h"

namespace leslie {

std::string ToStr(const BitboardType bitboard) {
  char str[Board::GetStrBoardSize()];
  char *ptr = str;

  for (BitboardType sq = Board::Start(); Board::IsEnd(sq);
       sq = Board::Next(sq)) {
    *ptr++ = bitboard & sq ? '1' : '0';
    if (Board::IsEndLine(sq)) *ptr++ = '\n';
  }
  *ptr = '\0';
  return std::string{str};
}

Position::Position(const std::string &fen)
    : pieces_(),
      en_passant_(),
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

  iss >> board_part >> turn_part >> castling_part >> en_passant_part >>
      rule_50_ >> moves_;

  BitboardType i = Board::Start();
  for (const char c : board_part) {
    if (c == '/') continue;

    if (isdigit(c)) {
      i = Board::Shift(i, c - '0');
      continue;
    }

    Piece piece = CharToPiece(c);
    BitboardType &bitboard = pieces_.GetBitboard(piece);

    bitboard |= i;
    i = Board::Next(i);
  }

  turn_ = (turn_part == "w") ? Color::kWhite : Color::kBlack;

  w_king_castle = castling_part.find('K') != std::string::npos;
  w_queen_castle = castling_part.find('Q') != std::string::npos;
  b_king_castle = castling_part.find('k') != std::string::npos;
  b_queen_castle = castling_part.find('q') != std::string::npos;

  if (en_passant_part != "-") {
    int file = 'h' - en_passant_part[0];
    int rank = en_passant_part[1] - '1';
    en_passant_ = (1ULL << (rank * 8 + file));
  }
}

Piece Position::CharToPiece(const char c) {
  Color color = isupper(c) ? Color::kWhite : Color::kBlack;
  PieceType piece_type = PieceType::kNone;

  if (c == 'k')
    piece_type = PieceType::kKing;
  else if (c == 'q')
    piece_type = PieceType::kQueen;
  else if (c == 'r')
    piece_type = PieceType::kRook;
  else if (c == 'b')
    piece_type = PieceType::kBishop;
  else if (c == 'n')
    piece_type = PieceType::kKnight;
  else if (c == 'p')
    piece_type = PieceType::kPawn;
  // TODO: add debug output message
  return Piece(piece_type, color);
}

char Position::PieceToChar(const Piece piece) {
  bool is_upper = piece.color == Color::kWhite;
  char c = '.';

  if (piece.type == PieceType::kKing)
    c = 'k';
  else if (piece.type == PieceType::kQueen)
    c = 'q';
  else if (piece.type == PieceType::kRook)
    c = 'r';
  else if (piece.type == PieceType::kBishop)
    c = 'b';
  else if (piece.type == PieceType::kKnight)
    c = 'n';
  else if (piece.type == PieceType::kPawn)
    c = 'p';
  return is_upper ? static_cast<char>(toupper(c)) : c;
}

Position::operator std::string() const {
  char str[Board::GetStrBoardSize()];
  char *ptr = str;

  for (BitboardType sq = Board::Start(); Board::IsEnd(sq);
       sq = Board::Next(sq)) {
    *ptr++ = PieceToChar(pieces_.WhatPieceOnSquare(sq));
    if (Board::IsEndLine(sq)) *ptr++ = '\n';
  }
  *ptr = '\0';
  return std::string{str};
}

Color Position::GetOpponent() const {
  return static_cast<Color>(std::to_underlying(turn_) ^ 0b1);
}

void Position::GetMoves(std::vector<Move> &vec) const {
  vec.clear();

  for (PieceType piece_type : kPieceTypes) {
    BitboardType bitboard = pieces_.GetBitboard({piece_type, turn_});
    AddMoves(&Position::AddKingMoves, bitboard, turn_, vec);
  }
}

void Position::AddMoves(void (Position::*adder)(BitboardType, Color,
                                                std::vector<Move> &) const,
                        BitboardType pieces, Color color,
                        std::vector<Move> &vec) const {
  while (pieces) {
    BitboardType position = 1ull << std::countr_zero(pieces);
    (this->*adder)(position, color, vec);
    pieces ^= position;
  }
}

void Position::AddPieceMoves(BitboardType from, BitboardType to, PieceType type,
                             std::vector<Move> &vec) const {
  while (to) {
    BitboardType position = 1ull << std::countr_zero(to);
    vec.emplace_back(type, from, position);
    to ^= position;
  }
}

void Position::AddKingMoves(BitboardType position, Color color,
                            std::vector<Move> &vec) const {
  BitboardType my_blockers = pieces_.GetBlockers(turn_);

  BitboardType a =
      (((position << 7) | (position >> 9) | (position >> 1)) & (~kFileA));
  BitboardType b =
      (((position >> 7) | (position << 9) | (position << 1)) & (~kFileH));
  BitboardType c = ((position >> 8) | (position << 8));
  BitboardType result = a | b | c;
  result &= ~my_blockers;
  AddPieceMoves(position, result, PieceType::kKing, vec);
}

void Position::AddQueenMoves(BitboardType position, Color color,
                             std::vector<Move> &vec) const {}

void Position::AddRookMoves(BitboardType position, Color color,
                            std::vector<Move> &vec) const {
  // const Engine& engine = Engine::Instance();
  // BitboardType my_blockers = pieces_.GetBlockers(turn_);
  // BitboardType op_blockers = pieces_.GetBlockers(GetOpponent());
  // BitboardType blockers = my_blockers | op_blockers;
  // BitboardType rook_mask = engine.[std::countr_zero(position)];
  //
  // auto filtered_blockers =
  //     static_cast<MagicKeyType>(_pext_u64(blockers, rook_mask));
  // BitboardType moves =
  // engine.GetMagic()[std::countr_zero(position)][filtered_blockers];
  // add_piece_moves(position, moves, PieceType::kRook, vec);
}

void Position::AddBishopMoves(BitboardType position, Color color,
                              std::vector<Move> &vec) const {
  // add_piece_moves(position, result, PieceType::BISHOP, vec);
}

void Position::AddKnightMoves(BitboardType position, Color color,
                              std::vector<Move> &vec) const {
  BitboardType my_blockers = pieces_.GetBlockers(turn_);

  BitboardType l1 = (position >> 1) & ~kFileA;
  BitboardType l2 = (position >> 2) & ~(kFileA | kFileB);
  BitboardType r1 = (position << 1) & ~kFileH;
  BitboardType r2 = (position << 2) & ~(kFileH | kFileG);
  BitboardType result =
      (l1 | r1) << 16 | (l1 | r1) >> 16 | (l2 | r2) << 8 | (l2 | r2) >> 8;
  result &= ~my_blockers;
  AddPieceMoves(position, result, PieceType::kKnight, vec);
}

void Position::AddPawnMoves(BitboardType position, Color color,
                            std::vector<Move> &vec) const {
  if (color == Color::kWhite)
    AddWhitePawnMoves(position, vec);
  else
    AddBlackPawnMoves(position, vec);
}

void Position::AddWhitePawnMoves(BitboardType position,
                                 std::vector<Move> &vec) const {
  BitboardType my_blockers = pieces_.GetBlockers(Color::kWhite);
  BitboardType op_blockers = pieces_.GetBlockers(Color::kBlack);
  BitboardType blockers = my_blockers | op_blockers;

  BitboardType short_moves = (position << 8) & ~blockers;
  BitboardType long_moves =
      ((position & kRank2) << 16) & ~blockers & (short_moves << 8);
  BitboardType attacks =
      op_blockers & (((position << 9) & ~kFileH) | ((position << 7) & ~kFileA));
  BitboardType result = short_moves | long_moves | attacks;

  AddPieceMoves(position, result, PieceType::kPawn, vec);
}

void Position::AddBlackPawnMoves(BitboardType position,
                                 std::vector<Move> &vec) const {
  BitboardType my_blockers = pieces_.GetBlockers(Color::kBlack);
  BitboardType op_blockers = pieces_.GetBlockers(Color::kWhite);
  BitboardType blockers = my_blockers | op_blockers;

  BitboardType short_moves = (position >> 8) & ~blockers;
  BitboardType long_moves =
      ((position & kRank7) >> 16) & ~blockers & (short_moves >> 8);
  BitboardType attacks =
      op_blockers & (((position >> 9) & ~kFileA) | ((position >> 7) & ~kFileH));
  BitboardType result = short_moves | long_moves | attacks;

  AddPieceMoves(position, result, PieceType::kPawn, vec);
}

}  // namespace leslie