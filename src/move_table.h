#ifndef LESLIE_MOVE_TABLE_H
#define LESLIE_MOVE_TABLE_H

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

class MoveTable {
 public:
  using MagicKeyType = uint16_t;

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
  static MoveTable& Instance();

  void RefreshValidMoves(const Position& position, std::vector<OrderManager::Data>& out);
  template <PieceType T>
  struct InsertWrapper {
    static void Insert(const Position& position, Color turn, BitboardType blockers, BitboardType valid,
               std::vector<OrderManager::Data>& out);
  };

  static BitboardType GetKingsMoves(BitboardType sqs, BitboardType blockers);
  BitboardType GetQueensMoves(BitboardType sqs, BitboardType blockers) const;
  BitboardType GetRooksMoves(BitboardType sqs, BitboardType blockers) const;
  BitboardType GetBishopsMoves(BitboardType sqs, BitboardType blockers) const;
  static BitboardType GetKnightsMoves(BitboardType sqs, BitboardType blockers);
  static BitboardType GetPawnsMoves(BitboardType sqs, BitboardType blockers,
                                    BitboardType en_passant, Color color);

  BitboardType GetQueenMoves(BitboardType sq, BitboardType blockers) const;
  BitboardType GetRookMoves(BitboardType sq, BitboardType blockers) const;
  BitboardType GetBishopMoves(BitboardType sq, BitboardType blockers) const;

 private:
  using MovesGetter = BitboardType (MoveTable::*)(BitboardType, BitboardType) const;

  std::array<std::unordered_map<MagicKeyType, BitboardType>, Board::kSize> rook_magic_;
  std::array<std::unordered_map<MagicKeyType, BitboardType>, Board::kSize> bishop_magic_;
  std::array<BitboardType, Board::kSize> rook_masks_;
  std::array<BitboardType, Board::kSize> bishop_masks_;
  std::array<std::array<BitboardType, Board::kSize>, Board::kSideLength> rays_;

  MoveTable();

  BitboardType GetPieceMoves(MovesGetter getter, BitboardType sqs, BitboardType blockers) const;

  void InitMasks();
  void InitMagic(const MasksType& masks, MagicsType& magic, Direction d1, Direction d2,
                 Direction d3, Direction d4) const;
  BitboardType RayTracing(BitboardType blockers, Direction direction, int sq_index) const;
};

/*
 * MOVE TABLE DEFINITIONS
 */
template <>
inline void MoveTable::InsertWrapper<KING>::Insert(const Position& position, Color turn,
                                            BitboardType blockers, BitboardType valid,
                                            std::vector<OrderManager::Data>& out) {
  BitboardType res = (((sqs << 7) | (sqs >> 9) | (sqs >> 1)) & (~FILE_A));
  res |= (((sqs >> 7) | (sqs << 9) | (sqs << 1)) & (~FILE_H));
  res |= ((sqs >> 8) | (sqs << 8));
  return res;
}


BitboardType MoveTable::GetPawnsMoves(const BitboardType sqs, const BitboardType blockers,
                                      const BitboardType en_passant, const Color color) {
  if (color == WHITE) {
    const BitboardType short_moves = (sqs << 8) & ~blockers;
    const BitboardType long_moves = ((sqs & RANK_2) << 16) & ~blockers & (short_moves << 8);
    const BitboardType attacks =
        (blockers | en_passant) & (((sqs << 9) & ~FILE_H) | ((sqs << 7) & ~FILE_A));
    return short_moves | long_moves | attacks;
  }
  const BitboardType short_moves = (sqs >> 8) & ~blockers;
  const BitboardType long_moves = ((sqs & RANK_7) >> 16) & ~blockers & (short_moves >> 8);
  const BitboardType attacks =
      (blockers | en_passant) & (((sqs >> 9) & ~FILE_A) | ((sqs >> 7) & ~FILE_H));
  return short_moves | long_moves | attacks;
}

}  // namespace Leslie

#endif
