#ifndef LESLIE_TYPES_H_
#define LESLIE_TYPES_H_

#include <array>
#include <cinttypes>
#include <unordered_map>

#define LESLIE_ROW(sq1, sq2, sq3, sq4, sq5, sq6, sq7, sq8, rank)       \
  sq1 = kFileA & rank, sq2 = sq1 >> 1, sq3 = sq1 >> 2, sq4 = sq1 >> 3, \
  sq5 = sq1 >> 4, sq6 = sq1 >> 5, sq7 = sq1 >> 6, sq8 = sq1 >> 7

namespace leslie {

using BitboardType = uint64_t;
using CounterType = uint8_t;
using SizeType = size_t;
using MagicKeyType = uint16_t;
using MagicsType =
    std::array<std::unordered_map<MagicKeyType, BitboardType>, 64>;
using MasksType = std::array<BitboardType, 64>;
using RaysType = std::array<std::array<BitboardType, 64>, 8>;

constexpr const char* kStartFen =
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

constexpr BitboardType kRank1 = 0xFFull;
constexpr BitboardType kRank2 = kRank1 << (8 * 1);
constexpr BitboardType kRank3 = kRank1 << (8 * 2);
constexpr BitboardType kRank4 = kRank1 << (8 * 3);
constexpr BitboardType kRank5 = kRank1 << (8 * 4);
constexpr BitboardType kRank6 = kRank1 << (8 * 5);
constexpr BitboardType kRank7 = kRank1 << (8 * 6);
constexpr BitboardType kRank8 = kRank1 << (8 * 7);

constexpr BitboardType kFileA = 0x8080808080808080ull;
constexpr BitboardType kFileB = kFileA >> 1;
constexpr BitboardType kFileC = kFileA >> 2;
constexpr BitboardType kFileD = kFileA >> 3;
constexpr BitboardType kFileE = kFileA >> 4;
constexpr BitboardType kFileF = kFileA >> 5;
constexpr BitboardType kFileG = kFileA >> 6;
constexpr BitboardType kFileH = kFileA >> 7;

enum class Color {
  kWhite,
  kBlack,
};

constexpr std::array kColors{
    Color::kWhite,
    Color::kBlack,
};

enum class PieceType {
  kNone = -1,
  kKing,
  kQueen,
  kRook,
  kBishop,
  kKnight,
  kPawn,
};

constexpr std::array kPieceTypes = {
    PieceType::kKing,   PieceType::kQueen,  PieceType::kRook,
    PieceType::kBishop, PieceType::kKnight, PieceType::kPawn,
};

struct Piece {
  PieceType type;
  Color color;
};

enum class Square : BitboardType {
  LESLIE_ROW(kA8, kB8, kC8, kD8, kE8, kF8, kG8, kH8, kRank8),
  LESLIE_ROW(kA7, kB7, kC7, kD7, kE7, kF7, kG7, kH7, kRank7),
  LESLIE_ROW(kA6, kB6, kC6, kD6, kE6, kF6, kG6, kH6, kRank6),
  LESLIE_ROW(kA5, kB5, kC5, kD5, kE5, kF5, kG5, kH5, kRank5),
  LESLIE_ROW(kA4, kB4, kC4, kD4, kE4, kF4, kG4, kH4, kRank4),
  LESLIE_ROW(kA3, kB3, kC3, kD3, kE3, kF3, kG3, kH3, kRank3),
  LESLIE_ROW(kA2, kB2, kC2, kD2, kE2, kF2, kG2, kH2, kRank2),
  LESLIE_ROW(kA1, kB1, kC1, kD1, kE1, kF1, kG1, kH1, kRank1),
};

enum class Direction {
  kUp,
  kUpRight,
  kRight,
  kDownRight,
  kDown,
  kDownLeft,
  kLeft,
  kUpLeft,
};

}  // namespace leslie

#endif
