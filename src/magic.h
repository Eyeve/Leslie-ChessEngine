#ifndef MAGIC_H
#define MAGIC_H

#include <unordered_map>

#include "types.h"


namespace Leslie
{
	static std::unordered_map<blockers_mask, bitboard> rook_magic[64];
	static std::unordered_map<blockers_mask, bitboard> bishop_magic[64];

	static bitboard RookMasks[64];
	static bitboard BishopMasks[64];

	inline void init_magic()
	{
		// TODO: move to another file
	}

	inline void init_masks()
	{
		// TODO: move to another file

		bitboard north_ray = FileH ^ Square::H1;
		bitboard south_ray = FileA ^ Square::A8;
		bitboard east_ray = Rank1 ^ Square::H1;
		bitboard west_ray = Rank8 ^ Square::A8;

		for (int sq = 0; sq < 64; ++sq)
		{
			bitboard n = north_ray << sq;
			bitboard s = south_ray >> (63 - sq);
			bitboard e = (east_ray & (east_ray >> sq % 8)) << sq;
			bitboard w = (west_ray & (west_ray << 7 - sq % 8)) >> sq;
			RookMasks[sq] = n | s | e | w;
			// TODO: add diag rays
		}
	}
}

#endif
