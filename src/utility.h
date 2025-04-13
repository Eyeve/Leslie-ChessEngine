#ifndef LESLIE_TYPES_H_
#define LESLIE_TYPES_H_

#include <array>
#include <cinttypes>
#include <string>
#include <unordered_map>

#include "piece.h"

namespace Leslie {

using BitboardType = uint64_t;
using CounterType = uint8_t;
using SizeType = size_t;
using MagicKeyType = uint16_t;
using MagicsType = std::array<std::unordered_map<MagicKeyType, BitboardType>, 64>;
using MasksType = std::array<BitboardType, 64>;
using RaysType = std::array<std::array<BitboardType, 64>, 8>;

std::string ToStr(BitboardType bitboard);

class Position;
std::string ToStr(const Position& position);

Piece ConvertCharToPiece(char c);
char ConvertPieceToChar(Piece piece);

}  // namespace Leslie

#endif
