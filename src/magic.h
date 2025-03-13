#ifndef MAGIC_H
#define MAGIC_H

#include <unordered_map>

#include "types.h"


namespace Leslie
{
	static std::unordered_map<blockers_mask, bitboard> RookMagic[64];
	static std::unordered_map<blockers_mask, bitboard> BishopMagic[64];

	static bitboard RookMasks[64];
	static bitboard BishopMasks[64];

	inline void init_magic()
	{
		// TODO: move to another file
	}

	inline void init_masks()
	{
		// TODO: move to another file

		bitboard up_ray = FileH ^ Square::H1;
		bitboard down_ray = FileA ^ Square::A8;
		bitboard right_ray = Rank8 ^ Square::A8;
		bitboard left_ray = Rank1 ^ Square::H1;

		bitboard up_right_ray = Square::B2 | Square::C3 | Square::D4 | Square::E5 | Square::F6 | Square::G7 | Square::H8;
		bitboard down_left_ray = Square::G7 | Square::F6 | Square::E5 | Square::D4 | Square::C3 | Square::B2 | Square::A1;
		bitboard up_left_ray = Square::H2 | Square::G3 | Square::F4 | Square::E5 | Square::D6 | Square::C7 | Square::B8;
		bitboard down_right_ray = Square::A7 | Square::B6 | Square::C5 | Square::D4 | Square::E3 | Square::F2 | Square::G1;



		for (int sq = 0; sq < 64; ++sq)
		{
			bitboard up = up_ray << sq;
			bitboard down = down_ray >> (63 - sq);
			bitboard left = (left_ray & (left_ray >> sq % 8)) << sq;
			bitboard right = (right_ray & (right_ray << 7 - sq % 8)) >> (63 - sq);
			RookMasks[sq] = up | down | left | right;

			bitboard up_right = (up_right_ray & (up_right_ray << (9 * (sq % 8)))) << sq;
			bitboard down_left = (down_left_ray & (down_left_ray >> (9 * (7 - sq % 8)))) >> (63 - sq);
			bitboard up_left = (up_left_ray & (up_left_ray << (7 * (sq % 8)))) << sq;
			bitboard down_right = (down_right_ray & (down_right_ray >> (7 * (7 - sq % 8)))) >> (63 - sq);

			BishopMasks[sq] = up_right | down_left | up_left | down_right;
		}
	}
}

#endif
