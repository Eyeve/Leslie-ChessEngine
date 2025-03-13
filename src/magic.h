#ifndef MAGIC_H
#define MAGIC_H

#include <unordered_map>

#include "types.h"


namespace Leslie
{
	static std::unordered_map<blockers_mask, bitboard> RookMagic[64];
	static std::unordered_map<blockers_mask, bitboard> BishopMagic[64];

	static bool IsMasksInitialized = false;
	static bool IsMagicInitialized = false;

	static bitboard RookMasks[64];
	static bitboard BishopMasks[64];

	static bitboard Rays[8][64];


	inline void init_magic()
	{
		// TODO: move to another file

        for (bitboard mask : RookMasks)
        {
        	bitboard limit = 1ull << std::popcount(mask);
        	for (bitboard src = 0ull; src < limit; ++src)
        	{
        		// bitboard blockers = _pdep_u64(src, mask);

        	}
        }
	}

	inline void init_masks()
	{
		// TODO: move to another file

		/*
		 *         y
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
				const bitboard center = 1ull << i;
				bitboard up = 0ull;
				bitboard right = 0ull;
				bitboard down = 0ull;
				bitboard left = 0ull;
				bitboard up_right = 0ull;
				bitboard up_left = 0ull;
				bitboard down_right = 0ull;
				bitboard down_left = 0ull;

				for (int sqX = 0; sqX < 8; ++sqX)
				{
					for (int sqY = 0; sqY < 8; ++sqY)
					{
						bitboard sq = 1ull << (sqX + sqY * 8);
						const bool lower = sq < center;
						const bool bigger = sq > center;

						if (centerX == sqX)
						{
							if (lower) down |= sq;
                            else if (bigger) up |= sq;
						}
						if (centerY == sqY)
						{
							if (lower) right |= sq;
							else if (bigger) left |= sq;
						}
						if (sqY == sqX + (centerY - centerX)) // positive k
						{
							if (lower) down_left |= sq;
							else if (bigger) up_right |= sq;
						}
						if (sqY == -sqX + (centerY + centerX)) // negative k
						{
							if (lower) down_right |= sq;
							else if (bigger) up_left |= sq;
						}
					}
				}

				Rays[Direction::UP][i] = up;
				Rays[Direction::RIGHT][i] = right;
				Rays[Direction::DOWN][i] = down;
				Rays[Direction::LEFT][i] = left;
				Rays[Direction::UP_RIGHT][i] = up_right;
				Rays[Direction::UP_LEFT][i] = up_left;
				Rays[Direction::DOWN_RIGHT][i] = down_right;
				Rays[Direction::DOWN_LEFT][i] = down_left;
                RookMasks[i] = up | right | down | left;
				BishopMasks[i] = up_right | up_left | down_right | down_left;
			}
		}

		IsMasksInitialized = true;
	}
}

#endif
