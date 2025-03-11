//
// Created by Eyeve on 10.03.2025.
//

#ifndef MOVE_H
#define MOVE_H

#include "types.h"

#include <functional>

namespace Leslie
{
	struct Move
	{

		inline Move(PieceType piece, Color color, bitboard from, bitboard to, PieceType transformation) :
			piece(piece), color(color), from(from), to(to), transformation(transformation) {}

		PieceType piece;
		Color color;
		bitboard from;
		bitboard to;
		PieceType transformation;
		
		bool operator==( const Move &other) const
		{
			return this->piece == other.piece &&
				   this->color == other.color &&
				   this->from == other.from &&
				   this->to == other.to &&
				   this->transformation == other.transformation;
		}
	};

	struct MoveHash
	{
		std::size_t operator()(const Move &m) const
		{
			std::size_t hash = 0;
			hash ^= std::hash<int>()(m.piece) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			hash ^= std::hash<int>()(m.color) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			hash ^= std::hash<bitboard>()(m.from) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			hash ^= std::hash<bitboard>()(m.to) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			hash ^= std::hash<int>()(m.transformation) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			return hash;
		}
	};
}	 // namespace Leslie

#endif	  // MOVE_H
