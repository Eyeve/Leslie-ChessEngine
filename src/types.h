//
// Created by Eyeve on 11.03.2025.
//

#ifndef COLOR_H
#define COLOR_H

#include <cinttypes>


namespace Leslie
{
	using bitboard = uint64_t;
	using counter = uint8_t;
	using size_type = size_t;

	enum Color
	{
		WHITE,
		BLACK,
	};

	enum PieceType
	{
		KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN, NONE,
	};

	enum Piece
	{
		W_KING, W_QUEEN, W_ROOK, W_BISHOP, W_KNIGHT, W_PAWN,
		B_KING, B_QUEEN, B_ROOK, B_BISHOP, B_KNIGHT, B_PAWN,
	};

	enum Square
	{
		A1, A2, A3, A4, A5, A6, A7, A8,
		B1, B2, B3, B4, B5, B6, B7, B8,
		C1, C2, C3, C4, C5, C6, C7, C8,
		D1, D2, D3, D4, D5, D6, D7, D8,
		E1, E2, E3, E4, E5, E6, E7, E8,
		F1, F2, F3, F4, F5, F6, F7, F8,
		G1, G2, G3, G4, G5, G6, G7, G8,
		H1, H2, H3, H4, H5, H6, H7, H8,
	};
}




#endif //COLOR_H
