#ifndef MOVE_H
#define MOVE_H

#include <functional>

#include "types.h"


namespace Leslie
{
	struct Move
	{
		PieceType type;
		bitboard from;
		bitboard to;

		Move(PieceType type, bitboard from, bitboard to);

		bool operator==(const Move &other) const = default;
	};

	struct MoveHash
	{
		std::size_t operator()(const Move &obj) const;
	};
}

#endif
