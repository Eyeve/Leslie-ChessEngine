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
		Move(PieceType piece, Color color, bitboard from, bitboard to, PieceType transformation):
            piece(piece), color(color), from(from), to(to), transformation(transformation) {}

		PieceType piece;
		Color color;
		bitboard from;
		bitboard to;
		PieceType transformation;
    };
}


#endif //MOVE_H
