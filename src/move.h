#ifndef LESLIE_MOVE_H_
#define LESLIE_MOVE_H_

#include "board.h"

#define LESLIE_IDENTITY(h, g, f, e, d, c, b, a) h, g, f, e, d, c, b, a
#define LESLIE_FROM_SHIFT 0
#define LESLIE_TO_SHIFT 6
#define LESLIE_EN_PASSANT_SHIFT 12
#define LESLIE_CASTLING_SHIFT 13
#define LESLIE_TYPE_SHIFT 14
#define LESLIE_ATTACK_SHIFT 17
#define LESLIE_CAPTURE_SHIFT 18
#define LESLIE_PROMOTION_SHIFT 21
#define LESLIE_CHECK_SHIFT 24

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

/**
 * Move Representation (32-bit packed format)
 *
 * This structure encodes all necessary information about a chess move
 * into a single 32-bit unsigned integer for performance and memory efficiency.
 *
 * Bit layout (from least significant to most significant bit):
 *
 *                  ---> bigger
 *               |     13 bits     |
 * 000000 000000 X X XXX X XXX XXX X 0000000
 *
 * Bits  0 -  5   (6 bits):  From square (0–63)
 * Bits  6 - 11   (6 bits):  To square (0–63)
 * Bit     12     (1 bit):   En passant flag (1 if the move is an en passant capture)
 * Bit     13     (1 bit):   Castling flag (1 if the move is a castling move)
 * Bits 14 - 16   (3 bits):  Moving piece type (look PieceType enum)
 * Bit     17     (1 bit):   Attacked flag (1 if this move attacks an enemy piece)
 * Bits 18 - 20   (3 bits):  Captured piece type (king if no piece captured))
 * Bits 21 - 23   (3 bits):  Promotion piece type (king if no promotion occurred)
 * Bit     24     (1 bit):   Check flag (1 if the move gives check)
 * Bits 25 - 31   (7 bits):  Reserved for future use
 *
 * This encoding enables fast access and comparison of moves
 * while supporting detailed move properties such as captures,
 * promotions, special moves (castling, en passant), and tactical flags.
 *
 * The bits are arranged in such a way as to give a rough estimate of the move,
 * the higher the value, the more promising the move.
 */

enum SquareShift : uint8_t {
  LESLIE_IDENTITY(H1, G1, F1, E1, D1, C1, B1, A1),
  LESLIE_IDENTITY(H2, G2, F2, E2, D2, C2, B2, A2),
  LESLIE_IDENTITY(H3, G3, F3, E3, D3, C3, B3, A3),
  LESLIE_IDENTITY(H4, G4, F4, E4, D4, C4, B4, A4),
  LESLIE_IDENTITY(H5, G5, F5, E5, D5, C5, B5, A5),
  LESLIE_IDENTITY(H6, G6, F6, E6, D6, C6, B6, A6),
  LESLIE_IDENTITY(H7, G7, F7, E7, D7, C7, B7, A7),
  LESLIE_IDENTITY(H8, G8, F8, E8, D8, C8, B8, A8),
};

using Move = uint32_t;

class MoveHandler {
 public:
  MoveHandler() = delete;

  static Move EncodeMove(PieceType type, SquareShift from, SquareShift to, PieceType capture,
                         PieceType promotion, bool en_passant, bool castling, bool attack,
                         bool check);
  static PieceType GetType(Move move);
  static Square GetFrom(Move move);
  static Square GetTo(Move move);
  static PieceType GetCapture(Move move);
  static PieceType GetPromotion(Move move);
  static bool GetEnPassant(Move move);
  static bool GetCastling(Move move);
  static bool GetAttack(Move move);
  static bool GetCheck(Move move);

 private:
  static Move EncodeMoveProcessing(Move type, Move from, Move to, Move capture, Move promotion,
                                   Move en_passant, Move castling, Move attack, Move check);
};

}  // namespace Leslie

#endif
