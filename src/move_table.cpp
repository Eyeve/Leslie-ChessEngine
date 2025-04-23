#include "move_table.h"

#include <bmi2intrin.h>

#include <algorithm>

namespace Leslie {

std::vector<OrderManager::Data>& OrderManager::GetData() { return data_; }

const std::vector<OrderManager::Data>& OrderManager::GetData() const { return data_; }

void OrderManager::Update(const Position& position) {
  MoveTable mt = MoveTable::Instance();
  data_.clear();
  mt.RefreshValidMoves(position, data_);
}

void OrderManager::Sort() {
  static auto comparator = [](const Data& lhs, const Data& rhs) -> bool {
    return lhs.estimation < rhs.estimation;
  };
  std::ranges::sort(data_, comparator);
}

void MoveTable::Init() { Instance(); }

MoveTable& MoveTable::Instance() {
  static MoveTable singleton;
  return singleton;
}

void MoveTable::RefreshValidMoves(const Position& position, std::vector<OrderManager::Data>& out) {
  const Color turn = position.GetMyColor();
  const BitboardType my_blockers = position.GetMyBlockers();
  const BitboardType op_blockers = position.GetOpBlockers();
  const BitboardType valid = ~my_blockers;
  const BitboardType blockers = my_blockers | op_blockers;

  InsertWrapper<KING>::Insert(position, turn, blockers, valid, out);
  InsertWrapper<QUEEN>::Insert(position, turn, blockers, valid, out);
  InsertWrapper<ROOK>::Insert(position, turn, blockers, valid, out);
  InsertWrapper<BISHOP>::Insert(position, turn, blockers, valid, out);
  InsertWrapper<KNIGHT>::Insert(position, turn, blockers, valid, out);
  InsertWrapper<PAWN>::Insert(position, turn, blockers, valid, out);
}

template <PieceType T>
void MoveTable::InsertMoveWrapper(const Position& position, const Color turn, const BitboardType blockers,
                        const BitboardType valid, std::vector<OrderManager::Data>& out) {
  BitboardType from = position.GetPieceContainer().GetBitboard(Piece(T, turn));
  while (from) {
    const BitboardType from_sq = 1ull << std::countr_zero(from);

    BitboardType to = InsertMove<T>(position, from_sq, blockers) & valid;
    while (to) {
      const BitboardType to_sq = 1ull << std::countr_zero(to);
      const Move move(type, from_sq, to_sq);

      if (MakeMove(move).IsMoveMadeValid()) moves.push_back(move);
      to ^= to_sq;
    }
    from ^= from_sq;
  }
}

BitboardType MoveTable::GetKingsMoves(const BitboardType sqs, const BitboardType blockers) {
  BitboardType res = (((sqs << 7) | (sqs >> 9) | (sqs >> 1)) & (~FILE_A));
  res |= (((sqs >> 7) | (sqs << 9) | (sqs << 1)) & (~FILE_H));
  res |= ((sqs >> 8) | (sqs << 8));
  return res;
}

BitboardType MoveTable::GetQueensMoves(const BitboardType sqs, const BitboardType blockers) const {
  return GetPieceMoves(&MoveTable::GetQueenMoves, sqs, blockers);
}

BitboardType MoveTable::GetRooksMoves(const BitboardType sqs, const BitboardType blockers) const {
  return GetPieceMoves(&MoveTable::GetRookMoves, sqs, blockers);
}

BitboardType MoveTable::GetBishopsMoves(const BitboardType sqs, const BitboardType blockers) const {
  return GetPieceMoves(&MoveTable::GetBishopMoves, sqs, blockers);
}

BitboardType MoveTable::GetKnightsMoves(const BitboardType sqs, const BitboardType blockers) {
  const BitboardType l1 = (sqs >> 1) & ~FILE_A;
  const BitboardType l2 = (sqs >> 2) & ~(FILE_A | FILE_B);
  const BitboardType r1 = (sqs << 1) & ~FILE_H;
  const BitboardType r2 = (sqs << 2) & ~(FILE_H | FILE_G);
  return (l1 | r1) << 16 | (l1 | r1) >> 16 | (l2 | r2) << 8 | (l2 | r2) >> 8;
}

BitboardType MoveTable::GetQueenMoves(const BitboardType sq, const BitboardType blockers) const {
  return GetRookMoves(sq, blockers) | GetBishopMoves(sq, blockers);
}

BitboardType MoveTable::GetRookMoves(const BitboardType sq, const BitboardType blockers) const {
  const int index = std::countr_zero(sq);
  const BitboardType mask = rook_masks_[index];
  const auto key = static_cast<MagicKeyType>(_pext_u64(blockers, mask));
  return rook_magic_[index].at(key);
}

BitboardType MoveTable::GetBishopMoves(const BitboardType sq, const BitboardType blockers) const {
  const int index = std::countr_zero(sq);
  const BitboardType mask = bishop_masks_[index];
  const auto key = static_cast<MagicKeyType>(_pext_u64(blockers, mask));
  return bishop_magic_[index].at(key);
}

MoveTable::MoveTable() : rook_masks_(), bishop_masks_(), rays_() {
  InitMasks();
  InitMagic(rook_masks_, rook_magic_, UP, RIGHT, DOWN, LEFT);
  InitMagic(bishop_masks_, bishop_magic_, UP_RIGHT, UP_LEFT, DOWN_RIGHT, DOWN_LEFT);
}

BitboardType MoveTable::GetPieceMoves(MovesGetter getter, BitboardType sqs,
                                      BitboardType blockers) const {
  BitboardType res = 0ull;
  while (sqs) {
    const BitboardType sq = 1ull << std::countr_zero(sqs);
    res |= (this->*getter)(sq, blockers);
    sqs ^= sq;
  }
  return res;
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
            if (lower) down |= sq;
            if (bigger) up |= sq;
          }
          if (center_y == sq_y) {
            if (lower) right |= sq;
            if (bigger) left |= sq;
          }
          if (sq_y == sq_x + center_y - center_x) {
            if (lower) down_right |= sq;
            if (bigger) up_left |= sq;
          }
          if (sq_y == -sq_x + center_y + center_x) {
            if (lower) down_left |= sq;
            if (bigger) up_right |= sq;
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
      magic[i][src] = r1 | r2 | r3 | r4;
    }
  }
}

BitboardType MoveTable::RayTracing(const BitboardType blockers, const Direction direction,
                                   const int sq_index) const {
  const BitboardType ray = rays_[direction][sq_index];
  const BitboardType ray_blockers = blockers & ray;

  if (ray_blockers == 0ull) return ray;

  const bool is_least_bit =
      direction == LEFT | direction == UP_LEFT | direction == UP | direction == UP_RIGHT;
  const int index =
      is_least_bit ? std::countr_zero(ray_blockers) : 63 - std::countl_zero(ray_blockers);
  const BitboardType blocked = rays_[direction][index];
  return ray ^ blocked;
}

}  // namespace Leslie
