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

		for (int sq = 0; sq < 64; ++sq)
		{
			bitboard up = up_ray << sq;
			bitboard down = down_ray >> (63 - sq);
			bitboard left = (left_ray & (left_ray >> sq % 8)) << sq;
			bitboard right = (right_ray & (right_ray << 7 - sq % 8)) >> (63 - sq);
			RookMasks[sq] = up | down | left | right;
			// TODO: add diag rays
		}
	}
}

#endif
