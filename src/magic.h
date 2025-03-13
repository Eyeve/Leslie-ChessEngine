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

		/*         y
		 *         ^
		 *    0000 |
		 *    0000 |
		 *    0000 |
		 * x <-----o
		 */

		for (int centerX = 0; centerX < 8; ++centerX)
		{
			for (int centerY = 0; centerY < 8; ++centerY)
			{
				int i = centerX + centerY * 8;
				bitboard center = 1ull << i;
				bitboard rookMask = 0ull;
				bitboard bishopMask = 0ull;

				for (int sqX = 0; sqX < 8; ++sqX)
				{
					for (int sqY = 0; sqY < 8; ++sqY)
					{
						bitboard sq = 1ull << (sqX + sqY * 8);
						if (centerX == sqX || centerY == sqY)
							rookMask |= sq;
						if (sqY == sqX + (centerY - centerX) || sqY == -sqX + (centerY + centerX))
							bishopMask |= sq;
					}
				}

                RookMasks[i] = rookMask ^ center;
				BishopMasks[i] = bishopMask ^ center;
			}
		}
	}
}

#endif
