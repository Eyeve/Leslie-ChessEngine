#include "board.h"


namespace Leslie
{
	std::string to_str(bitboard board)
	{
		// TODO: implementation
		uint64_t mask = 0xFF;
		return "";
	}

	bitboard Board::get_white_pawn_advances()
    {
        return (whitePawns << 8) | (whitePawns & Rank2) << 16;
    }

    bitboard Board::get_black_pawn_advances()
    {
        return (blackPawns >> 8) | (blackPawns & Rank7) >> 16;
    }
}
