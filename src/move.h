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
		Move(PieceType piece, Color color, Square from, Square to, PieceType type): piece(piece), color(color), from(from), to(to), type(type) {}

		PieceType piece;
		Color color;
		Square from;
		Square to;
		PieceType type;
    };
}


#endif //MOVE_H
