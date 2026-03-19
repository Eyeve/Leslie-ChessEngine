#ifndef LESLIE_MOVE_H_
#define LESLIE_MOVE_H_

#include <cstddef>
#include <cstdint>
#include <functional>

#include "board.h"

namespace Leslie {

enum Direction {
  UP,
  UP_RIGHT,
  RIGHT,
  DOWN_RIGHT,
  DOWN,
  DOWN_LEFT,
  LEFT,
  UP_LEFT,
};

struct Move {
  PieceType type = NONE_PIECE_TYPE;
  BitboardType from = 0ull;
  BitboardType to = 0ull;
  PieceType capture = NONE_PIECE_TYPE;
  PieceType promotion = NONE_PIECE_TYPE;
  bool en_passant = false;
  bool castling = false;
  bool check = false;
  uint8_t attack_est = 0;

  constexpr Move() = default;

  constexpr Move(const PieceType move_type, const BitboardType move_from,
                 const BitboardType move_to,
                 const PieceType move_capture = NONE_PIECE_TYPE,
                 const PieceType move_promotion = NONE_PIECE_TYPE,
                 const bool move_en_passant = false,
                 const bool move_castling = false,
                 const bool move_check = false,
                 const uint8_t move_attack_est = 0)
      : type(move_type),
        from(move_from),
        to(move_to),
        capture(move_capture),
        promotion(move_promotion),
        en_passant(move_en_passant),
        castling(move_castling),
        check(move_check),
        attack_est(move_attack_est) {}

  friend bool operator==(const Move& lhs, const Move& rhs) {
    return lhs.type == rhs.type && lhs.from == rhs.from && lhs.to == rhs.to;
  }
};

struct MoveHash {
  std::size_t operator()(const Move& move) const noexcept {
    const std::size_t h1 = std::hash<int>()(static_cast<int>(move.type));
    const std::size_t h2 = std::hash<BitboardType>()(move.from);
    const std::size_t h3 = std::hash<BitboardType>()(move.to);
    return h1 ^ (h2 << 1) ^ (h3 << 2);
  }
};

}  // namespace Leslie

#endif
