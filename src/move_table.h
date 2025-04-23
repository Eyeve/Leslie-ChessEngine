#ifndef LESLIE_MOVE_TABLE_H
#define LESLIE_MOVE_TABLE_H

#include <bmi2intrin.h>

#include <array>
#include <cstdint>
#include <vector>

#include "board.h"
#include "move.h"
#include "position.h"
#include "utility.h"

namespace Leslie {

/**
 * @class OrderManager
 * @brief Manages and sorts moves based on custom estimation (move type estimation by default).
 *
 * The OrderManager class stores move-related data (position and evaluation)
 * and provides functionality to access, update, and sort this data. It is
 * useful in scenarios such as move ordering for optimization in a chess engine.
 */
class OrderManager {
 public:
  struct Data {
    Position position;
    Move move = 0;
    EstimationType estimation = 0;
  };

  OrderManager() = default;
  /**
   * @brief Returns a reference to the internal data vector.
   *
   * This allows modification of the stored move data.
   *
   * @return std::vector<Data>& Reference to the data vector.
   */
  std::vector<Data>& GetData();
  /**
   * @brief Returns a const reference to the internal data vector.
   *
   * This allows read-only access to the stored move data.
   *
   * @return const std::vector<Data>& Const reference to the data vector.
   */
  const std::vector<Data>& GetData() const;
  /**
   * @brief Checks whether a position is available for reading.
   *
   * Returns true if the internal reading pointer is within bounds of the data vector.
   *
   * @return true if there is data to read, false otherwise.
   */
  void Update(const Position& position);
  /**
   * @brief Sorts the stored data based on estimation values.
   *
   * This can be used to prioritize moves based on evaluation scores.
   */
  void Sort();

 private:
  std::vector<Data> data_;
};

struct MoveTable {
  using MagicKeyType = uint16_t;

  std::array<std::unordered_map<MagicKeyType, BitboardType>, Board::kSize> rook_magic;
  std::array<std::unordered_map<MagicKeyType, BitboardType>, Board::kSize> bishop_magic;
  std::array<BitboardType, Board::kSize> rook_masks;
  std::array<BitboardType, Board::kSize> bishop_masks;
  std::array<std::array<BitboardType, Board::kSize>, Board::kSideLength> rays;

  static void Init();
  /**
   * @brief Returns a reference to the single instance of the MoveTable.
   *
   * This static method implements the Singleton pattern, providing access to
   * the unique instance of the `MoveTable` class. If the instance has not
   * been created yet, it will be initialized on the first call to this method.
   *
   * @return MoveTable& reference to the single instance of the MoveTable.
   */
  static const MoveTable& Instance();

  void RefreshValidMoves(const Position& position, std::vector<OrderManager::Data>& out);

 private:
  MoveTable();

  void InitMasks();
  void InitMagic(const MasksType& masks, MagicsType& magic, Direction d1, Direction d2,
                 Direction d3, Direction d4) const;
  BitboardType RayTracing(BitboardType blockers, Direction direction, int sq_index) const;
};

template <PieceType T>
class PieceHandler {
 public:
  PieceHandler() = delete;
  static void Insert(const Position& position, Color turn, BitboardType blockers,
                     BitboardType valid, std::vector<OrderManager::Data>& out);
  static BitboardType GetDestinations(BitboardType sqs, BitboardType blockers,
                                      BitboardType en_passant, Color turn);
  static BitboardType GetDestination(BitboardType sq, BitboardType blockers,
                                     BitboardType en_passant, Color turn);

 private:
  static BitboardType Multiplier(BitboardType sqs, BitboardType blockers, BitboardType en_passant,
                                 Color turn);
};

/*
 * PIECE HANDLER DEFINITIONS
 */

template <PieceType T>
void PieceHandler<T>::Insert(const Position& position, BitboardType my_blockers, Color turn,
                             BitboardType op_blockers, BitboardType valid,
                             std::vector<OrderManager::Data>& out) {
  static BitboardType promotion_cells = RANK_1 | RANK_8;
  BitboardType from = position.GetPieceContainer().GetBitboard(Piece(T, turn));
  while (from) {
    const BitboardType from_sq = 1ull << std::countr_zero(from);

    // Move type, Move from, Move to, Move capture, Move promotion,
    //                                Move en_passant, Move castling, Move attack, Move check
    BitboardType to = GetDestination(from_sq, blockers, en_passant, turn) & valid;
    while (to) {
      const BitboardType to_sq = 1ull << std::countr_zero(to);
      const bool capture = to_sq & op_blockers;
      bool promotion = false;
      bool en_passant_capture = false;
      bool castling = false;
      GetDestination(to_sq, blockers, 0, turn);

      if constexpr (T == PAWN) {
        promotion = to_sq & promotion_cells;
        en_passant_capture = to_sq == en_passant;
      } else if constexpr (T == KING) {
        castling = (from_sq << 2) == to_sq || (from_sq >> 2) == to_sq;
      }
      Move move = MoveHandler::EncodeMove(T, from_sq, to_sq, capture, promotion, en_passant_capture,
                                          castling);
      const Position updated = position.MakeMove(move);
      if (!updated.IsKingSafe(turn)) continue;
      const BitboardType updated_blockers = updated.GetMyBlockers() | updated.GetOpBlockers();

      PieceHandler<KING>::GetDestinations(updated.GetOpBitboard(KING), updated_blockers, 0, turn);
      PieceHandler<QUEEN>::GetDestinations(updated.GetOpBitboard(QUEEN), updated_blockers, 0, turn);
      PieceHandler<ROOK>::GetDestinations(updated.GetOpBitboard(ROOK), updated_blockers, 0, turn);
      PieceHandler<BISHOP>::GetDestinations(updated.GetOpBitboard(BISHOP), updated_blockers, 0, turn);
      PieceHandler<KNIGHT>::GetDestinations(updated.GetOpBitboard(KNIGHT), updated_blockers, 0, turn);
      PieceHandler<PAWN>::GetDestinations(updated.GetOpBitboard(PAWN), updated_blockers, 0, turn);

      if (MakeMove(move).IsMoveMadeValid()) moves.push_back(move);
      to ^= to_sq;
    }
    from ^= from_sq;
  }
}

template <PieceType T>
BitboardType PieceHandler<T>::GetDestinations(const BitboardType sqs, const BitboardType blockers,
                                              const BitboardType en_passant, const Color turn) {
  return GetDestination(sqs, blockers, turn, en_passant, turn);
}

template <PieceType T>
BitboardType PieceHandler<T>::Multiplier(BitboardType sqs, const BitboardType blockers,
                                         const BitboardType en_passant, const Color turn) {
  BitboardType res = 0ull;
  while (sqs) {
    const BitboardType sq = 1ull << std::countr_zero(sqs);
    res |= GetDestination(sq, blockers, en_passant, turn);
    sqs ^= sq;
  }
  return res;
}

template <>
BitboardType PieceHandler<KING>::GetDestination(const BitboardType sq, const BitboardType blockers,
                                                const BitboardType en_passant, const Color turn) {
  BitboardType des = (((sq << 7) | (sq >> 9) | (sq >> 1)) & (~FILE_A));
  des |= (((sq >> 7) | (sq << 9) | (sq << 1)) & (~FILE_H));
  des |= ((sq >> 8) | (sq << 8));
  return des;
}

template <>
BitboardType PieceHandler<QUEEN>::GetDestinations(const BitboardType sqs,
                                                  const BitboardType blockers,
                                                  const BitboardType en_passant, const Color turn) {
  return Multiplier(sqs, blockers, en_passant, turn);
}

template <>
BitboardType PieceHandler<QUEEN>::GetDestination(const BitboardType sq, const BitboardType blockers,
                                                 const BitboardType en_passant, const Color turn) {
  return PieceHandler<ROOK>::GetDestination(sq, blockers, en_passant, turn) |
         PieceHandler<BISHOP>::GetDestination(sq, blockers, en_passant, turn);
}

template <>
BitboardType PieceHandler<ROOK>::GetDestinations(const BitboardType sqs,
                                                 const BitboardType blockers,
                                                 const BitboardType en_passant, const Color turn) {
  return Multiplier(sqs, blockers, en_passant, turn);
}

template <>
BitboardType PieceHandler<ROOK>::GetDestination(const BitboardType sq, const BitboardType blockers,
                                                const BitboardType en_passant, const Color turn) {
  const int index = std::countr_zero(sq);
  const MoveTable& mt = MoveTable::Instance();
  const BitboardType mask = mt.rook_masks[index];
  const auto key = static_cast<MagicKeyType>(_pext_u64(blockers, mask));
  return mt.rook_magic[index].at(key);
}

template <>
BitboardType PieceHandler<BISHOP>::GetDestinations(const BitboardType sqs,
                                                   const BitboardType blockers,
                                                   const BitboardType en_passant,
                                                   const Color turn) {
  return Multiplier(sqs, blockers, en_passant, turn);
}

template <>
BitboardType PieceHandler<BISHOP>::GetDestination(const BitboardType sq,
                                                  const BitboardType blockers,
                                                  const BitboardType en_passant, const Color turn) {
  const int index = std::countr_zero(sq);
  const MoveTable& mt = MoveTable::Instance();
  const BitboardType mask = mt.bishop_masks[index];
  const auto key = static_cast<MagicKeyType>(_pext_u64(blockers, mask));
  return mt.bishop_magic[index].at(key);
}

template <>
BitboardType PieceHandler<KNIGHT>::GetDestination(const BitboardType sq,
                                                  const BitboardType blockers,
                                                  const BitboardType en_passant, const Color turn) {
  const BitboardType l1 = (sq >> 1) & ~FILE_A;
  const BitboardType l2 = (sq >> 2) & ~(FILE_A | FILE_B);
  const BitboardType r1 = (sq << 1) & ~FILE_H;
  const BitboardType r2 = (sq << 2) & ~(FILE_H | FILE_G);
  return (l1 | r1) << 16 | (l1 | r1) >> 16 | (l2 | r2) << 8 | (l2 | r2) >> 8;
}

template <>
BitboardType PieceHandler<PAWN>::GetDestination(const BitboardType sq, const BitboardType blockers,
                                                const BitboardType en_passant, const Color turn) {
  if (turn == WHITE) {
    const BitboardType short_moves = (sq << 8) & ~blockers;
    const BitboardType long_moves = ((sq & RANK_2) << 16) & ~blockers & (short_moves << 8);
    const BitboardType attacks =
        (blockers | en_passant) & (((sq << 9) & ~FILE_H) | ((sq << 7) & ~FILE_A));
    return short_moves | long_moves | attacks;
  }
  const BitboardType short_moves = (sq >> 8) & ~blockers;
  const BitboardType long_moves = ((sq & RANK_7) >> 16) & ~blockers & (short_moves >> 8);
  const BitboardType attacks =
      (blockers | en_passant) & (((sq >> 9) & ~FILE_A) | ((sq >> 7) & ~FILE_H));
  return short_moves | long_moves | attacks;
}

}  // namespace Leslie

#endif
