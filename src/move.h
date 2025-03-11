//
// Created by Eyeve on 10.03.2025.
//

#ifndef MOVE_H
#define MOVE_H

#include "types.h"


namespace Leslie
{
	struct Move
	{
		Move(Piece piece, Square from, Square to, PieceType type): piece(piece), from(from), to(to), type(type) {}

		Piece piece;
		Square from;
		Square to;
		PieceType type;
    };
}


#endif //MOVE_H
