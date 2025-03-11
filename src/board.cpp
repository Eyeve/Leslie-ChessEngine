#include "board.h"


namespace Leslie
{
	std::string to_str(bitboard board)
	{
		char result[72];
		char* ptr = result;

		for (int rank = 7; rank >= 0; --rank) {
			for (int file = 7; file >= 0; --file) {
				*ptr++ = (board & (1ULL << (rank * 8 + file))) ? '1' : '0';
			}
			*ptr++ = '\n';
		}
		*ptr = '\0';

		return std::string(result);
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
