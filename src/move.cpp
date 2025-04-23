#include "move.h"

#include <utility>

namespace Leslie {

Move MoveHandler::EncodeMove(const PieceType type, const SquareShift from, const SquareShift to,
                             const PieceType capture, const PieceType promotion,
                             const bool en_passant, const bool castling, const bool attack,
                             const bool check) {
  return EncodeMoveProcessing(type, from, to, capture, promotion, en_passant, castling, attack,
                              check);
}

Move MoveHandler::EncodeMoveProcessing(const Move type, const Move from, const Move to,
                                       const Move capture, const Move promotion,
                                       const Move en_passant, const Move castling,
                                       const Move attack, const Move check) {
  return (type << LESLIE_TYPE_SHIFT) | (from << LESLIE_FROM_SHIFT) | (to << LESLIE_TO_SHIFT) |
         (capture << LESLIE_CAPTURE_SHIFT) | (promotion << LESLIE_PROMOTION_SHIFT) |
         (en_passant << LESLIE_EN_PASSANT_SHIFT) | (castling << LESLIE_CASTLING_SHIFT) |
         (attack << LESLIE_ATTACK_SHIFT) | (check << LESLIE_CHECK_SHIFT);
}

}  // namespace Leslie
