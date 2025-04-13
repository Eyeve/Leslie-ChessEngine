#include "utility.h"

#include <stdexcept>

#include "board.h"
#include "position.h"

namespace Leslie {

EstimationType GetPieceCost(PieceType type) {
  switch (type) {
    case KING:
      return 0;
    case QUEEN:
      return 900;
    case ROOK:
      return 500;
    case BISHOP:
      return 310;
    case KNIGHT:
      return 300;
    case PAWN:
      return 100;
    default:
      throw std::runtime_error("Unknown type");
  }
}

std::string ToStr(const BitboardType bitboard) {
  char str[Board::kStrFormatSize];
  char* ptr = str;

  for (BitboardType sq = Board::Start(); Board::IsEnd(sq); sq = Board::Next(sq)) {
    *ptr++ = bitboard & sq ? '1' : '0';
    if (Board::IsEndLine(sq)) *ptr++ = '\n';
  }
  *ptr = '\0';
  return str;
}

std::string ToStr(const Position& position) {
  char str[Board::kStrFormatSize];
  char* ptr = str;

  for (BitboardType sq = Board::Start(); Board::IsEnd(sq); sq = Board::Next(sq)) {
    *ptr++ = ConvertPieceToChar(position.WhatPieceOnSquare(sq));
    if (Board::IsEndLine(sq)) *ptr++ = '\n';
  }
  *ptr = '\0';
  return str;
}

Piece ConvertCharToPiece(char c) {
  const Color color = isupper(c) ? WHITE : BLACK;
  PieceType piece_type = NONE_PIECE_TYPE;

  const char symbol = static_cast<char>(tolower(c));
  if (symbol == 'k')
    piece_type = KING;
  else if (symbol == 'q')
    piece_type = QUEEN;
  else if (symbol == 'r')
    piece_type = ROOK;
  else if (symbol == 'b')
    piece_type = BISHOP;
  else if (symbol == 'n')
    piece_type = KNIGHT;
  else if (symbol == 'p')
    piece_type = PAWN;
  // TODO: add debug output message
  return Piece(piece_type, color);
}

char ConvertPieceToChar(Piece piece) {
  const bool is_upper = piece.color == WHITE;
  char c = '.';

  if (piece.type == KING)
    c = 'k';
  else if (piece.type == QUEEN)
    c = 'q';
  else if (piece.type == ROOK)
    c = 'r';
  else if (piece.type == BISHOP)
    c = 'b';
  else if (piece.type == KNIGHT)
    c = 'n';
  else if (piece.type == PAWN)
    c = 'p';
  return is_upper ? static_cast<char>(toupper(c)) : c;
}

}  // namespace Leslie
