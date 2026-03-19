#include "move_table.h"

#include <immintrin.h>

#include <algorithm>
#include <bit>

namespace Leslie {

std::vector<OrderManager::Data>& OrderManager::GetData() { return data_; }

const std::vector<OrderManager::Data>& OrderManager::GetData() const { return data_; }

void OrderManager::Update(const Position& position) {
  const MoveTable& mt = MoveTable::Instance();
  data_.clear();
  mt.RefreshValidMoves(position, data_);
}

void OrderManager::Sort() {
  static auto comparator = [](const Data& lhs, const Data& rhs) -> bool {
    return lhs.estimation < rhs.estimation;
  };
  std::ranges::sort(data_, comparator);
}

void MoveTable::Init() { (void)Instance(); }

const MoveTable& MoveTable::Instance() {
  static MoveTable singleton;
  return singleton;
}

const MasksType& MoveTable::GetRookMasks() const { return rook_masks_; }

const MasksType& MoveTable::GetBishopMasks() const { return bishop_masks_; }

const MagicsType& MoveTable::GetRookMagic() const { return rook_magic_; }

const MagicsType& MoveTable::GetBishopMagic() const { return bishop_magic_; }

MoveTable::MoveTable() {
  InitMasks();
  InitMagic(rook_masks_, rook_magic_, UP, RIGHT, DOWN, LEFT);
  InitMagic(bishop_masks_, bishop_magic_, UP_RIGHT, UP_LEFT, DOWN_RIGHT, DOWN_LEFT);
}

BitboardType MoveTable::PopLeastBit(BitboardType& bb) {
  const BitboardType sq = 1ull << std::countr_zero(bb);
  bb ^= sq;
  return sq;
}

BitboardType MoveTable::GetKingDestinations(const BitboardType sq) {
  BitboardType des = (((sq << 7) | (sq >> 9) | (sq >> 1)) & (~FILE_A));
  des |= (((sq >> 7) | (sq << 9) | (sq << 1)) & (~FILE_H));
  des |= ((sq >> 8) | (sq << 8));
  return des;
}

BitboardType MoveTable::GetKnightDestinations(const BitboardType sq) {
  const BitboardType l1 = (sq >> 1) & ~FILE_A;
  const BitboardType l2 = (sq >> 2) & ~(FILE_A | FILE_B);
  const BitboardType r1 = (sq << 1) & ~FILE_H;
  const BitboardType r2 = (sq << 2) & ~(FILE_H | FILE_G);
  return ((l1 | r1) << 16) | ((l1 | r1) >> 16) | ((l2 | r2) << 8) | ((l2 | r2) >> 8);
}

BitboardType MoveTable::GetKingsMoves(BitboardType sqs, const BitboardType blockers) const {
  (void)blockers;
  BitboardType res = 0ull;
  while (sqs) {
    const BitboardType sq = PopLeastBit(sqs);
    res |= GetKingDestinations(sq);
  }
  return res;
}

BitboardType MoveTable::GetRooksMoves(BitboardType sqs, const BitboardType blockers) const {
  BitboardType res = 0ull;
  while (sqs) {
    const BitboardType sq = PopLeastBit(sqs);
    const int index = std::countr_zero(sq);
    const BitboardType mask = rook_masks_[index];
    const auto key = static_cast<MagicKeyType>(_pext_u64(blockers, mask));
    const auto it = rook_magic_[index].find(key);
    if (it != rook_magic_[index].end()) {
      res |= it->second;
    }
  }
  return res;
}

BitboardType MoveTable::GetBishopsMoves(BitboardType sqs, const BitboardType blockers) const {
  BitboardType res = 0ull;
  while (sqs) {
    const BitboardType sq = PopLeastBit(sqs);
    const int index = std::countr_zero(sq);
    const BitboardType mask = bishop_masks_[index];
    const auto key = static_cast<MagicKeyType>(_pext_u64(blockers, mask));
    const auto it = bishop_magic_[index].find(key);
    if (it != bishop_magic_[index].end()) {
      res |= it->second;
    }
  }
  return res;
}

BitboardType MoveTable::GetQueensMoves(const BitboardType sqs, const BitboardType blockers) const {
  return GetRooksMoves(sqs, blockers) | GetBishopsMoves(sqs, blockers);
}

BitboardType MoveTable::GetKnightsMoves(BitboardType sqs, const BitboardType blockers) const {
  (void)blockers;
  BitboardType res = 0ull;
  while (sqs) {
    const BitboardType sq = PopLeastBit(sqs);
    res |= GetKnightDestinations(sq);
  }
  return res;
}

BitboardType MoveTable::GetPawnsMoves(BitboardType sqs, const BitboardType blockers,
                                      const BitboardType en_passant, const Color turn) const {
  BitboardType result = 0ull;
  while (sqs) {
    const BitboardType sq = PopLeastBit(sqs);

    if (turn == WHITE) {
      const BitboardType short_moves = (sq << 8) & ~blockers;
      const BitboardType long_moves = ((sq & RANK_2) << 16) & ~blockers & (short_moves << 8);
      const BitboardType attacks =
          (blockers | en_passant) & (((sq << 9) & ~FILE_H) | ((sq << 7) & ~FILE_A));
      result |= short_moves | long_moves | attacks;
      continue;
    }

    const BitboardType short_moves = (sq >> 8) & ~blockers;
    const BitboardType long_moves = ((sq & RANK_7) >> 16) & ~blockers & (short_moves >> 8);
    const BitboardType attacks =
        (blockers | en_passant) & (((sq >> 9) & ~FILE_A) | ((sq >> 7) & ~FILE_H));
    result |= short_moves | long_moves | attacks;
  }
  return result;
}

BitboardType MoveTable::GetPawnAttacks(BitboardType sqs, const Color turn) const {
  if (turn == WHITE) {
    return ((sqs << 9) & ~FILE_H) | ((sqs << 7) & ~FILE_A);
  }
  return ((sqs >> 9) & ~FILE_A) | ((sqs >> 7) & ~FILE_H);
}

bool MoveTable::IsSquareAttacked(const Position& position, const BitboardType square,
                                 const Color by_color) const {
  const BitboardType blockers = position.GetPieceContainer().GetBlockers(WHITE) |
                                position.GetPieceContainer().GetBlockers(BLACK);

  BitboardType attacks = 0ull;
  attacks |= GetKingsMoves(position.GetPieceContainer().GetBitboard(Piece(KING, by_color)), blockers);
  attacks |=
      GetQueensMoves(position.GetPieceContainer().GetBitboard(Piece(QUEEN, by_color)), blockers);
  attacks |= GetRooksMoves(position.GetPieceContainer().GetBitboard(Piece(ROOK, by_color)), blockers);
  attacks |=
      GetBishopsMoves(position.GetPieceContainer().GetBitboard(Piece(BISHOP, by_color)), blockers);
  attacks |=
      GetKnightsMoves(position.GetPieceContainer().GetBitboard(Piece(KNIGHT, by_color)), blockers);
  attacks |= GetPawnAttacks(position.GetPieceContainer().GetBitboard(Piece(PAWN, by_color)), by_color);

  return (attacks & square) != 0ull;
}

void MoveTable::RefreshValidMoves(const Position& position,
                                  std::vector<OrderManager::Data>& out) const {
  const Color turn = position.GetMyColor();
  const BitboardType my_blockers = position.GetMyBlockers();
  const BitboardType op_blockers = position.GetOpBlockers();
  const BitboardType blockers = my_blockers | op_blockers;
  const BitboardType valid = ~my_blockers;
  const BitboardType en_passant = position.GetEnPassant();

  auto append_piece_moves = [&](const PieceType type) {
    BitboardType from_sqs = position.GetPieceContainer().GetBitboard(Piece(type, turn));

    while (from_sqs) {
      const BitboardType from_sq = PopLeastBit(from_sqs);
      BitboardType to_sqs = 0ull;

      if (type == KING) {
        to_sqs = GetKingsMoves(from_sq, blockers);

        const Color op_color = static_cast<Color>(turn ^ 0b1);
        if (turn == WHITE && from_sq == SQ_E1) {
          if (position.CanCastleKingSide(WHITE) &&
              (position.GetPieceContainer().GetBitboard(Piece(ROOK, WHITE)) & SQ_H1) != 0ull &&
              (blockers & (SQ_F1 | SQ_G1)) == 0ull &&
              !IsSquareAttacked(position, SQ_E1, op_color) &&
              !IsSquareAttacked(position, SQ_F1, op_color) &&
              !IsSquareAttacked(position, SQ_G1, op_color)) {
            to_sqs |= SQ_G1;
          }
          if (position.CanCastleQueenSide(WHITE) &&
              (position.GetPieceContainer().GetBitboard(Piece(ROOK, WHITE)) & SQ_A1) != 0ull &&
              (blockers & (SQ_B1 | SQ_C1 | SQ_D1)) == 0ull &&
              !IsSquareAttacked(position, SQ_E1, op_color) &&
              !IsSquareAttacked(position, SQ_D1, op_color) &&
              !IsSquareAttacked(position, SQ_C1, op_color)) {
            to_sqs |= SQ_C1;
          }
        } else if (turn == BLACK && from_sq == SQ_E8) {
          if (position.CanCastleKingSide(BLACK) &&
              (position.GetPieceContainer().GetBitboard(Piece(ROOK, BLACK)) & SQ_H8) != 0ull &&
              (blockers & (SQ_F8 | SQ_G8)) == 0ull &&
              !IsSquareAttacked(position, SQ_E8, op_color) &&
              !IsSquareAttacked(position, SQ_F8, op_color) &&
              !IsSquareAttacked(position, SQ_G8, op_color)) {
            to_sqs |= SQ_G8;
          }
          if (position.CanCastleQueenSide(BLACK) &&
              (position.GetPieceContainer().GetBitboard(Piece(ROOK, BLACK)) & SQ_A8) != 0ull &&
              (blockers & (SQ_B8 | SQ_C8 | SQ_D8)) == 0ull &&
              !IsSquareAttacked(position, SQ_E8, op_color) &&
              !IsSquareAttacked(position, SQ_D8, op_color) &&
              !IsSquareAttacked(position, SQ_C8, op_color)) {
            to_sqs |= SQ_C8;
          }
        }
      } else if (type == QUEEN) {
        to_sqs = GetQueensMoves(from_sq, blockers);
      } else if (type == ROOK) {
        to_sqs = GetRooksMoves(from_sq, blockers);
      } else if (type == BISHOP) {
        to_sqs = GetBishopsMoves(from_sq, blockers);
      } else if (type == KNIGHT) {
        to_sqs = GetKnightsMoves(from_sq, blockers);
      } else if (type == PAWN) {
        to_sqs = GetPawnsMoves(from_sq, blockers, en_passant, turn);
      }

      to_sqs &= valid;

      while (to_sqs) {
        const BitboardType to_sq = PopLeastBit(to_sqs);

        PieceType capture_type = position.WhatPieceOnSquare(to_sq).type;
        bool en_passant_capture = false;
        if (type == PAWN && to_sq == en_passant && capture_type == NONE_PIECE_TYPE) {
          en_passant_capture = true;
          capture_type = PAWN;
        }

        const bool castling =
            type == KING && (((from_sq >> 2) == to_sq) || ((from_sq << 2) == to_sq));

        PieceType promotion = NONE_PIECE_TYPE;
        if (type == PAWN && (to_sq & (RANK_1 | RANK_8)) != 0ull) {
          promotion = QUEEN;
        }

        Move move(type, from_sq, to_sq, capture_type, promotion, en_passant_capture, castling);
        Position updated = position.MakeMove(move);
        if (!updated.IsKingSafe(turn)) {
          continue;
        }

        out.push_back({updated, move, updated.GetSimpleEstimation()});
      }
    }
  };

  append_piece_moves(KING);
  append_piece_moves(QUEEN);
  append_piece_moves(ROOK);
  append_piece_moves(BISHOP);
  append_piece_moves(KNIGHT);
  append_piece_moves(PAWN);
}

void MoveTable::InitMasks() {
  for (int center_x = 0; center_x < 8; ++center_x) {
    for (int center_y = 0; center_y < 8; ++center_y) {
      const int i = center_x + center_y * 8;
      const BitboardType center = 1ull << i;
      BitboardType up = 0ull;
      BitboardType right = 0ull;
      BitboardType down = 0ull;
      BitboardType left = 0ull;
      BitboardType up_right = 0ull;
      BitboardType up_left = 0ull;
      BitboardType down_right = 0ull;
      BitboardType down_left = 0ull;

      for (int sq_x = 0; sq_x < 8; ++sq_x) {
        for (int sq_y = 0; sq_y < 8; ++sq_y) {
          const BitboardType sq = 1ull << (sq_x + sq_y * 8);
          const bool lower = sq < center;
          const bool bigger = sq > center;

          if (center_x == sq_x) {
            if (lower) {
              down |= sq;
            } else if (bigger) {
              up |= sq;
            }
          }

          if (center_y == sq_y) {
            if (lower) {
              right |= sq;
            } else if (bigger) {
              left |= sq;
            }
          }

          if (sq_y == sq_x + center_y - center_x) {
            if (lower) {
              down_right |= sq;
            } else if (bigger) {
              up_left |= sq;
            }
          }

          if (sq_y == -sq_x + center_y + center_x) {
            if (lower) {
              down_left |= sq;
            } else if (bigger) {
              up_right |= sq;
            }
          }
        }
      }

      rays_[UP][i] = up;
      rays_[RIGHT][i] = right;
      rays_[DOWN][i] = down;
      rays_[LEFT][i] = left;
      rays_[UP_RIGHT][i] = up_right;
      rays_[UP_LEFT][i] = up_left;
      rays_[DOWN_RIGHT][i] = down_right;
      rays_[DOWN_LEFT][i] = down_left;

      rook_masks_[i] = up | right | down | left;
      bishop_masks_[i] = up_right | up_left | down_right | down_left;
    }
  }
}

void MoveTable::InitMagic(const MasksType& masks, MagicsType& magic, const Direction d1,
                          const Direction d2, const Direction d3, const Direction d4) const {
  for (int i = 0; i < 64; ++i) {
    const BitboardType mask = masks[i];
    const BitboardType limit = 1ull << std::popcount(mask);

    for (BitboardType src = 0ull; src < limit; ++src) {
      const BitboardType blockers = _pdep_u64(src, mask);
      const BitboardType r1 = RayTracing(blockers, d1, i);
      const BitboardType r2 = RayTracing(blockers, d2, i);
      const BitboardType r3 = RayTracing(blockers, d3, i);
      const BitboardType r4 = RayTracing(blockers, d4, i);
      magic[i][static_cast<MagicKeyType>(src)] = r1 | r2 | r3 | r4;
    }
  }
}

BitboardType MoveTable::RayTracing(const BitboardType blockers, const Direction direction,
                                   const int sq_index) const {
  const BitboardType ray = rays_[direction][sq_index];
  const BitboardType ray_blockers = blockers & ray;

  if (ray_blockers == 0ull) {
    return ray;
  }

  const bool is_least_bit =
      direction == LEFT || direction == UP_LEFT || direction == UP || direction == UP_RIGHT;
  const int index =
      is_least_bit ? std::countr_zero(ray_blockers) : 63 - std::countl_zero(ray_blockers);
  const BitboardType blocked = rays_[direction][index];
  return ray ^ blocked;
}

}  // namespace Leslie
