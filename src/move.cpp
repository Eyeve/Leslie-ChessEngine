#include "move.h"

#include <utility>

namespace leslie {

Move::Move(PieceType type, Square from, Square to)
    : type(type), from(from), to(to) {}

Move::Move(PieceType type, BitboardType from, BitboardType to)
    : type(type),
      from(static_cast<Square>(from)),
      to(static_cast<Square>(to)) {}

std::size_t MoveHash::operator()(const Move& obj) const {
  std::size_t hash = 0;
  hash = HashCombine(hash, obj.type);
  hash = HashCombine(hash, obj.from);
  hash = HashCombine(hash, obj.to);
  return hash;
}

template <typename T>
std::size_t MoveHash::HashCombine(std::size_t seed, T value) const {
  static constexpr size_t kBitMixer = 0x9e3779b9;
  return seed ^ (std::hash<T>()(value) + kBitMixer + (seed << 6) + (seed >> 2));
}

}  // namespace leslie
