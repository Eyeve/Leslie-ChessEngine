#include "engine.h"

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
  for (BitboardType mask : masks_.RookMasks) {
    BitboardType limit = 1ull << std::popcount(mask);
    for (BitboardType src = 0ull; src < limit; ++src) {
      // BitboardType blockers = _pdep_u64(src, mask);
    }
  }
}

void Engine::InitMasks() {
  /*
   *       y
   *       ^
   *    00 |
   *    00 |
   * x <---o
   */

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

      masks_.RookMasks[i] = up | right | down | left;
      masks_.BishopMasks[i] = up_right | up_left | down_right | down_left;
    }
  }
}

Engine::Engine() : position_(kStartFen), options_(), masks_(), magic_() {
  InitMagic();
  InitMasks();
}

Engine::~Engine() {}

}  // namespace leslie