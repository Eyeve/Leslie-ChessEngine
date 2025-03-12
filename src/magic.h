#ifndef MAGIC_H
#define MAGIC_H

#include <unordered_map>

#include "types.h"


namespace Leslie
{
	namespace Magic
	{
		static std::unordered_map<blockers_mask, bitboard> rook_magic[64];
		static std::unordered_map<blockers_mask, bitboard> bishop_magic[64];

		void init_magic();
	}
}

#endif
