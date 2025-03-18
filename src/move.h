#ifndef LESLIE_MOVE_H_
#define LESLIE_MOVE_H_

#include <functional>

#include "types.h"

namespace leslie {

struct Move {
  PieceType type;
  Square from;
  Square to;

  Move(PieceType type, Square from, Square to);
  Move(PieceType type, BitboardType from, BitboardType to);

  bool operator==(const Move& other) const = default;
};

struct MoveHash {
  std::size_t operator()(const Move& obj) const;
  template<typename T>
  std::size_t HashCombine(std::size_t seed, T value) const;
};

}  // namespace leslie

#endif
