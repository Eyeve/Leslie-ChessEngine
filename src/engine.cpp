#include "engine.h"

#include <immintrin.h>

#include <utility>

namespace leslie {

Engine& Engine::Instance() {
  static Engine singleton;
  return singleton;
}
Position& Engine::GetPosition() { return position_; }

const Position& Engine::GetPosition() const { return position_; }

Masks& Engine::GetMasks() { return masks_; }

const Masks& Engine::GetMasks() const { return masks_; }

Magic& Engine::GetMagic() { return magic_; }

const Magic& Engine::GetMagic() const { return magic_; }

void Engine::InitMagic() {
  InitPieceMagic(masks_.rook_masks, Direction::kUp, Direction::kRight,
                 Direction::kDown, Direction::kLeft);
  InitPieceMagic(masks_.bishop_masks, Direction::kUpRight, Direction::kUpLeft,
                 Direction::kDownRight, Direction::kDownLeft);
}

void Engine::InitMasks() {
  for (int center_x = 0; center_x < 8; ++center_x) {
    for (int center_y = 0; center_y < 8; ++center_y) {
      int i = center_x + center_y * 8;
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
          BitboardType sq = 1ull << (sq_x + sq_y * 8);
          const bool lower = sq < center;
          const bool bigger = sq > center;

          if (center_x == sq_x) {
            if (lower)
              down |= sq;
            else if (bigger)
              up |= sq;
          }
          if (center_y == sq_y) {
            if (lower)
              right |= sq;
            else if (bigger)
              left |= sq;
          }
          if (sq_y == sq_x + (center_y - center_x)) {
            if (lower)
              down_right |= sq;
            else if (bigger)
              up_left |= sq;
          }
          if (sq_y == -sq_x + (center_y + center_x)) {
            if (lower)
              down_left |= sq;
            else if (bigger)
              up_right |= sq;
          }
        }
      }

      rays_[std::to_underlying(Direction::kUp)][i] = up;
      rays_[std::to_underlying(Direction::kRight)][i] = right;
      rays_[std::to_underlying(Direction::kDown)][i] = down;
      rays_[std::to_underlying(Direction::kLeft)][i] = left;
      rays_[std::to_underlying(Direction::kUpRight)][i] = up_right;
      rays_[std::to_underlying(Direction::kUpLeft)][i] = up_left;
      rays_[std::to_underlying(Direction::kDownRight)][i] = down_right;
      rays_[std::to_underlying(Direction::kDownLeft)][i] = down_left;

      masks_.rook_masks[i] = up | right | down | left;
      masks_.bishop_masks[i] = up_right | up_left | down_right | down_left;
    }
  }
}

void Engine::InitPieceMagic(const MasksType& masks, Direction d1, Direction d2,
                            Direction d3, Direction d4) {
  for (int i = 0; i < 64; ++i) {
    BitboardType mask = masks[i];
    BitboardType limit = 1ull << std::popcount(mask);

    for (BitboardType src = 0ull; src < limit; ++src) {
      BitboardType blockers = _pdep_u64(src, mask);
      BitboardType r1 = RayTracing(blockers, d1, i);
      BitboardType r2 = RayTracing(blockers, d2, i);
      BitboardType r3 = RayTracing(blockers, d3, i);
      BitboardType r4 = RayTracing(blockers, d4, i);
      magic_.rook_magic[i][blockers] = r1 | r2 | r3 | r4;
    }
  }
}

BitboardType Engine::RayTracing(BitboardType blockers, Direction direction,
                                int sq_index) {
  BitboardType ray = rays_[std::to_underlying(direction)][sq_index];
  BitboardType ray_blockers = blockers & ray;
  bool is_least_bit =
      direction == Direction::kLeft | direction == Direction::kUpLeft |
      direction == Direction::kUp | direction == Direction::kUpRight;
  int index = is_least_bit ? std::countr_zero(ray_blockers)
                           : std::countl_zero(ray_blockers);
  BitboardType blocked = rays_[std::to_underlying(Direction::kUp)][index];
  return ray ^ blocked;
}

Engine::Engine()
    : position_(kStartFen), options_(), magic_(), masks_(), rays_() {
  InitMasks();
  InitMagic();
}

Engine::~Engine() {}

}  // namespace leslie