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

Engine::Engine() : position_(kStartFen), options_(), masks_(), magic_() {}

Engine::~Engine() {}

}  // namespace leslie