#include "engine.h"

#include <utility>

#include "magic.h"

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
  for (BitboardType mask : masks_.RookMasks)
  {
    BitboardType limit = 1ull << std::popcount(mask);
    for (BitboardType src = 0ull; src < limit; ++src)
    {
      // BitboardType blockers = _pdep_u64(src, mask);
      
    }
  }
}

void Engine::InitMasks() {
  /*
		 *         y
		 *         ^
		 *    0000 |
		 *    0000 |
		 *    0000 |
		 * x <-----o
   */
  
  for (int centerX = 0; centerX < 8; ++centerX)
  {
    for (int centerY = 0; centerY < 8; ++centerY)
    {
      int i = centerX + centerY * 8;
      const BitboardType center = 1ull << i;
      BitboardType up = 0ull;
      BitboardType right = 0ull;
      BitboardType down = 0ull;
      BitboardType left = 0ull;
      BitboardType up_right = 0ull;
      BitboardType up_left = 0ull;
      BitboardType down_right = 0ull;
      BitboardType down_left = 0ull;

      for (int sqX = 0; sqX < 8; ++sqX)
      {
        for (int sqY = 0; sqY < 8; ++sqY)
        {
          BitboardType sq = 1ull << (sqX + sqY * 8);
          const bool lower = sq < center;
          const bool bigger = sq > center;

          if (centerX == sqX)
          {
            if (lower) down |= sq;
            else if (bigger) up |= sq;
          }
          if (centerY == sqY)
          {
            if (lower) right |= sq;
            else if (bigger) left |= sq;
          }
          if (sqY == sqX + (centerY - centerX))
          {
            if (lower) down_right |= sq;
            else if (bigger) up_left |= sq;
          }
          if (sqY == -sqX + (centerY + centerX))
          {
            if (lower) down_left |= sq;
            else if (bigger) up_right |= sq;
          }
        }
      }

      Rays[std::to_underlying(Direction::kUp)][i] = up;
      Rays[std::to_underlying(Direction::kRight)][i] = right;
      Rays[std::to_underlying(Direction::kDown)][i] = down;
      Rays[std::to_underlying(Direction::kLeft)][i] = left;
      Rays[std::to_underlying(Direction::kUpRight)][i] = up_right;
      Rays[std::to_underlying(Direction::kUpLeft)][i] = up_left;
      Rays[std::to_underlying(Direction::kDownRight)][i] = down_right;
      Rays[std::to_underlying(Direction::kDownLeft)][i] = down_left;
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