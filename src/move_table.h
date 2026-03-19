#ifndef LESLIE_MOVE_TABLE_H
#define LESLIE_MOVE_TABLE_H

#include <array>
#include <cstdint>
#include <vector>

#include "move.h"
#include "position.h"
#include "utility.h"

namespace Leslie {

class OrderManager {
 public:
  struct Data {
    Position position;
    Move move{};
    EstimationType estimation = 0;
  };

  OrderManager() = default;

  std::vector<Data>& GetData();
  const std::vector<Data>& GetData() const;

  void Update(const Position& position);
  void Sort();

 private:
  std::vector<Data> data_;
};

class MoveTable {
 public:
  using MagicKeyType = uint16_t;

  static void Init();
  static const MoveTable& Instance();

  void RefreshValidMoves(const Position& position,
                         std::vector<OrderManager::Data>& out) const;

  BitboardType GetKingsMoves(BitboardType sqs, BitboardType blockers) const;
  BitboardType GetQueensMoves(BitboardType sqs, BitboardType blockers) const;
  BitboardType GetRooksMoves(BitboardType sqs, BitboardType blockers) const;
  BitboardType GetBishopsMoves(BitboardType sqs, BitboardType blockers) const;
  BitboardType GetKnightsMoves(BitboardType sqs, BitboardType blockers) const;
  BitboardType GetPawnsMoves(BitboardType sqs, BitboardType blockers,
                             BitboardType en_passant, Color turn) const;
  BitboardType GetPawnAttacks(BitboardType sqs, Color turn) const;
  bool IsSquareAttacked(const Position& position, BitboardType square,
                        Color by_color) const;

  const MasksType& GetRookMasks() const;
  const MasksType& GetBishopMasks() const;
  const MagicsType& GetRookMagic() const;
  const MagicsType& GetBishopMagic() const;

 private:
  MoveTable();

  void InitMasks();
  void InitMagic(const MasksType& masks, MagicsType& magic, Direction d1,
                 Direction d2, Direction d3, Direction d4) const;
  BitboardType RayTracing(BitboardType blockers, Direction direction,
                          int sq_index) const;

  static BitboardType PopLeastBit(BitboardType& bb);
  static BitboardType GetKingDestinations(BitboardType sq);
  static BitboardType GetKnightDestinations(BitboardType sq);

  MagicsType rook_magic_{};
  MagicsType bishop_magic_{};
  MasksType rook_masks_{};
  MasksType bishop_masks_{};
  RaysType rays_{};
};

}  // namespace Leslie

#endif
