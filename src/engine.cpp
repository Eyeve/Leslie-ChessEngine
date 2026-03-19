#include "engine.h"

#include "board.h"

namespace Leslie {

Engine& Engine::Instance() {
  static Engine singleton;
  return singleton;
}

Engine::Engine() : Engine(Options{}) {}

Engine::Engine(const Options& options) : position_(Board::kStartFen), options_(options) {
  const MoveTable& mt = MoveTable::Instance();
  masks_.rook_masks = mt.GetRookMasks();
  masks_.bishop_masks = mt.GetBishopMasks();
  magic_.rook_magic = mt.GetRookMagic();
  magic_.bishop_magic = mt.GetBishopMagic();
}

void Engine::Go() {}

void Engine::Stop() {}

Position& Engine::GetPosition() { return position_; }

const Position& Engine::GetPosition() const { return position_; }

void Engine::SetPosition(const std::string& fen) { position_ = Position(fen); }

const Engine::Options& Engine::GetOptions() const { return options_; }

Engine::Options& Engine::GetOptions() { return options_; }

const Masks& Engine::GetMasks() const { return masks_; }

const Magic& Engine::GetMagic() const { return magic_; }

}  // namespace Leslie
