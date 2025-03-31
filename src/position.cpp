#include "position.h"

#include <immintrin.h>

#include <bit>
#include <sstream>
#include <utility>

#include "board.h"
#include "engine.h"

namespace leslie {

Position::Position(const std::string& fen)
    : pieces_(),
      en_passant_(),
      rule_50_(),
      moves_(),
      current_(),
      w_king_castle(),
      w_queen_castle(),
      b_king_castle(),
      b_queen_castle() {
  // TODO: add checks for invalid input
  std::istringstream iss(fen);
  std::string board_part, turn_part, castling_part, en_passant_part;

  iss >> board_part >> turn_part >> castling_part >> en_passant_part >>
      rule_50_ >> moves_;

  BitboardType sq = Board::Start();
  for (const char c : board_part) {
    if (c == '/') continue;

    if (isdigit(c)) {
      sq = Board::Shift(sq, c - '0');
      continue;
    }

    BitboardType& bitboard = pieces_.GetBitboard(ConvertCharToPiece(c));
    bitboard |= sq;
    sq = Board::Next(sq);
  }

  current_ = (turn_part == "w") ? Color::kWhite : Color::kBlack;

  w_king_castle = castling_part.find('K') != std::string::npos;
  w_queen_castle = castling_part.find('Q') != std::string::npos;
  b_king_castle = castling_part.find('k') != std::string::npos;
  b_queen_castle = castling_part.find('q') != std::string::npos;

  if (en_passant_part != "-") {
    const int file = 'h' - en_passant_part[0];
    const int rank = en_passant_part[1] - '1';
    en_passant_ = (1ULL << (rank * 8 + file));
  }
}

Color Position::GetMyColor() const { return current_; }

Color Position::GetOpColor() const {
  return static_cast<Color>(std::to_underlying(current_) ^ 0b1);
}

Piece Position::WhatPieceOnSquare(const BitboardType sq) const {
  return pieces_.WhatPieceOnSquare(sq);
}

void Position::AddPossibleMoves(std::vector<Move>& vec) const {
  AddPieceMoves(&GetKingsMoves, PieceType::kKing, vec);
  AddPieceMoves(&GetQueenMoves, PieceType::kQueen, vec);
  AddPieceMoves(&GetRookMoves, PieceType::kRook, vec);
  AddPieceMoves(&GetBishopMoves, PieceType::kBishop, vec);
  AddPieceMoves(&GetKnightsMoves, PieceType::kKnight, vec);
  if (current_ == Color::kWhite)
    AddPieceMoves(&GetWhitePawnsMoves, PieceType::kPawn, vec);
  else
    AddPieceMoves(&GetBlackPawnsMoves, PieceType::kPawn, vec);
}

Position Position::MakeMoves(const std::vector<Move>& moves) const {
  Position my_copy(*this);
  for (auto move : moves) my_copy.MakeMoveInPlace(move);
  return my_copy;
}

Position Position::MakeMove(const Move move) const {
  Position my_copy(*this);
  my_copy.MakeMoveInPlace(move);
  return my_copy;
}

bool Position::IsMoveMadeValid() const {
  const BitboardType king_sq = GetOpBitboard(PieceType::kKing);
  const BitboardType blockers =
      pieces_.GetBlockers(GetMyColor()) | pieces_.GetBlockers(GetOpColor());

  BitboardType moves = GetKingsMoves(GetMyBitboard(PieceType::kKing), blockers);
  moves |= GetQueensMoves(GetMyBitboard(PieceType::kQueen), blockers);
  moves |= GetRooksMoves(GetMyBitboard(PieceType::kRook), blockers);
  moves |= GetBishopsMoves(GetMyBitboard(PieceType::kBishop), blockers);
  moves |= GetKnightsMoves(GetMyBitboard(PieceType::kKnight), blockers);
  if (current_ == Color::kWhite)
    moves |= GetWhitePawnsMoves(GetMyBitboard(PieceType::kPawn), blockers);
  else
    moves |= GetBlackPawnsMoves(GetMyBitboard(PieceType::kPawn), blockers);
  return static_cast<bool>(~moves & king_sq);
}

void Position::MakeMoveInPlace(const Move move) {
  // TODO: pawn promoting implementation
  // TODO: en passant capture implementation
  const BitboardType from = std::to_underlying(move.from);
  const BitboardType to = std::to_underlying(move.to);
  const BitboardType valid = ~to;
  BitboardType& ref = GetMyBitboard(move.type);

  ref &= ~from;
  ref |= to;
  GetOpBitboard(PieceType::kKing) &= valid;
  GetOpBitboard(PieceType::kQueen) &= valid;
  GetOpBitboard(PieceType::kRook) &= valid;
  GetOpBitboard(PieceType::kBishop) &= valid;
  GetOpBitboard(PieceType::kKnight) &= valid;
  GetOpBitboard(PieceType::kPawn) &= valid;
  ++moves_;
  current_ = GetOpColor();
}

void Position::AddPieceMoves(const MovesGetter getter, const PieceType type,
                             std::vector<Move>& moves) const {
  // TODO: move general parts outside
  const BitboardType my_blockers = pieces_.GetBlockers(GetMyColor());
  const BitboardType op_blockers = pieces_.GetBlockers(GetOpColor());
  const BitboardType valid_squares = ~my_blockers;
  const BitboardType blockers = my_blockers | op_blockers;

  BitboardType from = GetMyBitboard(type);
  while (from) {
    const BitboardType from_sq = 1ull << std::countr_zero(from);

    BitboardType to = (this->*getter)(from_sq, blockers) & valid_squares;
    while (to) {
      const BitboardType to_sq = 1ull << std::countr_zero(to);
      const Move move(type, from_sq, to_sq);

      if (MakeMove(move).IsMoveMadeValid()) moves.push_back(move);
      to ^= to_sq;
    }
    from ^= from_sq;
  }
}

const BitboardType& Position::GetMyBitboard(const PieceType type) const {
  return pieces_.GetBitboard(Piece(type, GetMyColor()));
}

const BitboardType& Position::GetOpBitboard(const PieceType type) const {
  return pieces_.GetBitboard(Piece(type, GetOpColor()));
}

BitboardType& Position::GetMyBitboard(const PieceType type) {
  return pieces_.GetBitboard(Piece(type, GetMyColor()));
}

BitboardType& Position::GetOpBitboard(const PieceType type) {
  return pieces_.GetBitboard(Piece(type, GetOpColor()));
}

BitboardType Position::GetKingsMoves(const BitboardType sqs,
                                     const BitboardType blockers) const {
  BitboardType res = (((sqs << 7) | (sqs >> 9) | (sqs >> 1)) & (~kFileA));
  res |= (((sqs >> 7) | (sqs << 9) | (sqs << 1)) & (~kFileH));
  res |= ((sqs >> 8) | (sqs << 8));
  return res;
}

BitboardType Position::GetQueensMoves(const BitboardType sqs,
                                      const BitboardType blockers) const {
  return GetPieceMoves(&GetQueenMoves, sqs, blockers);
}

BitboardType Position::GetRooksMoves(const BitboardType sqs,
                                     const BitboardType blockers) const {
  return GetPieceMoves(&GetRookMoves, sqs, blockers);
}

BitboardType Position::GetBishopsMoves(const BitboardType sqs,
                                       const BitboardType blockers) const {
  return GetPieceMoves(&GetBishopMoves, sqs, blockers);
}

BitboardType Position::GetKnightsMoves(const BitboardType sqs,
                                       const BitboardType blockers) const {
  const BitboardType l1 = (sqs >> 1) & ~kFileA;
  const BitboardType l2 = (sqs >> 2) & ~(kFileA | kFileB);
  const BitboardType r1 = (sqs << 1) & ~kFileH;
  const BitboardType r2 = (sqs << 2) & ~(kFileH | kFileG);
  return (l1 | r1) << 16 | (l1 | r1) >> 16 | (l2 | r2) << 8 | (l2 | r2) >> 8;
}

BitboardType Position::GetWhitePawnsMoves(const BitboardType sqs,
                                          const BitboardType blockers) const {
  const BitboardType short_moves = (sqs << 8) & ~blockers;
  const BitboardType long_moves =
      ((sqs & kRank2) << 16) & ~blockers & (short_moves << 8);
  const BitboardType attacks =
      (blockers | en_passant_) &
      (((sqs << 9) & ~kFileH) | ((sqs << 7) & ~kFileA));
  return short_moves | long_moves | attacks;
}
BitboardType Position::GetBlackPawnsMoves(const BitboardType sqs,
                                          const BitboardType blockers) const {
  const BitboardType short_moves = (sqs >> 8) & ~blockers;
  const BitboardType long_moves =
      ((sqs & kRank7) >> 16) & ~blockers & (short_moves >> 8);
  const BitboardType attacks =
      (blockers | en_passant_) &
      (((sqs >> 9) & ~kFileA) | ((sqs >> 7) & ~kFileH));
  return short_moves | long_moves | attacks;
}

BitboardType Position::GetQueenMoves(const BitboardType sq,
                                     const BitboardType blockers) const {
  return GetRookMoves(sq, blockers) | GetBishopMoves(sq, blockers);
}

BitboardType Position::GetRookMoves(const BitboardType sq,
                                    const BitboardType blockers) const {
  Engine& engine = Engine::Instance();
  const int index = std::countr_zero(sq);
  const BitboardType mask = engine.GetMasks().rook_masks[index];
  const auto key = static_cast<MagicKeyType>(_pext_u64(blockers, mask));
  return engine.GetMagic().rook_magic[index][key];
}

BitboardType Position::GetBishopMoves(const BitboardType sq,
                                      const BitboardType blockers) const {
  Engine& engine = Engine::Instance();
  const int index = std::countr_zero(sq);
  const BitboardType mask = engine.GetMasks().bishop_masks[index];
  const auto key = static_cast<MagicKeyType>(_pext_u64(blockers, mask));
  return engine.GetMagic().bishop_magic[index][key];
}

BitboardType Position::GetPieceMoves(MovesGetter getter, BitboardType sqs,
                                     BitboardType blockers) const {
  BitboardType res = 0ull;
  while (sqs) {
    const BitboardType sq = 1ull << std::countr_zero(sqs);
    res |= (this->*getter)(sq, blockers);
    sqs ^= sq;
  }
  return res;
}

}  // namespace leslie
