#include "types.h"

#include "board.h"
#include "position.h"

namespace Leslie {

std::string ToStr(const BitboardType bitboard) {
  char str[Board::GetStrBoardSize()];
  char* ptr = str;

  for (BitboardType sq = Board::Start(); Board::IsEnd(sq);
       sq = Board::Next(sq)) {
    *ptr++ = bitboard & sq ? '1' : '0';
    if (Board::IsEndLine(sq)) *ptr++ = '\n';
  }
  *ptr = '\0';
  return str;
}

std::string ToStr(const Position& position) {
  char str[Board::GetStrBoardSize()];
  char* ptr = str;

  for (BitboardType sq = Board::Start(); Board::IsEnd(sq);
       sq = Board::Next(sq)) {
    *ptr++ = ConvertPieceToChar(position.WhatPieceOnSquare(sq));
    if (Board::IsEndLine(sq)) *ptr++ = '\n';
  }
  *ptr = '\0';
  return str;
}

Piece ConvertCharToPiece(char c) {
  const Color color = isupper(c) ? Color::kWhite : Color::kBlack;
  PieceType piece_type = PieceType::kNone;

  const char symbol = static_cast<char>(tolower(c));
  if (symbol == 'k')
    piece_type = PieceType::kKing;
  else if (symbol == 'q')
    piece_type = PieceType::kQueen;
  else if (symbol == 'r')
    piece_type = PieceType::kRook;
  else if (symbol == 'b')
    piece_type = PieceType::kBishop;
  else if (symbol == 'n')
    piece_type = PieceType::kKnight;
  else if (symbol == 'p')
    piece_type = PieceType::kPawn;
  // TODO: add debug output message
  return Piece(piece_type, color);
}

char ConvertPieceToChar(Piece piece) {
  const bool is_upper = piece.color == Color::kWhite;
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

}  // namespace Leslie
