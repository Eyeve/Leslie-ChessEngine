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

const MoveTable& MoveTable::Instance() {
  static MoveTable singleton;
  return singleton;
}

void MoveTable::RefreshValidMoves(const Position& position, std::vector<OrderManager::Data>& out) {
  const Color turn = position.GetMyColor();
  const BitboardType my_blockers = position.GetMyBlockers();
  const BitboardType op_blockers = position.GetOpBlockers();
  const BitboardType valid = ~my_blockers;
  const BitboardType blockers = my_blockers | op_blockers;

  PieceHandler<KING>::Insert(position, turn, blockers, valid, out);
  PieceHandler<QUEEN>::Insert(position, turn, blockers, valid, out);
  PieceHandler<ROOK>::Insert(position, turn, blockers, valid, out);
  PieceHandler<BISHOP>::Insert(position, turn, blockers, valid, out);
  PieceHandler<KNIGHT>::Insert(position, turn, blockers, valid, out);
  PieceHandler<PAWN>::Insert(position, turn, blockers, valid, out);
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

      rays[UP][i] = up;
      rays[RIGHT][i] = right;
      rays[DOWN][i] = down;
      rays[LEFT][i] = left;
      rays[UP_RIGHT][i] = up_right;
      rays[UP_LEFT][i] = up_left;
      rays[DOWN_RIGHT][i] = down_right;
      rays[DOWN_LEFT][i] = down_left;
      rook_masks[i] = up | right | down | left;
      bishop_masks[i] = up_right | up_left | down_right | down_left;
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
  const BitboardType ray = rays[direction][sq_index];
  const BitboardType ray_blockers = blockers & ray;

  if (ray_blockers == 0ull) return ray;

  const bool is_least_bit =
      direction == LEFT | direction == UP_LEFT | direction == UP | direction == UP_RIGHT;
  const int index =
      is_least_bit ? std::countr_zero(ray_blockers) : 63 - std::countl_zero(ray_blockers);
  const BitboardType blocked = rays[direction][index];
  return ray ^ blocked;
}

}  // namespace Leslie
