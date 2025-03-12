#include "magic.h"

namespace Leslie::Magic
{
	void init_magic()
	{

	}

	void init_masks()
	{
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
