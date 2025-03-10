//
// Created by Eyeve on 10.03.2025.
//

#include "board.h"


Board::bitboard Board::get_white_pawn_advances()
{
	return (whitePawns << 8) | (whitePawns & Rank_2) << 16;
}

Board::bitboard Board::get_black_pawn_advances()
{
	return (blackPawns >> 8) | (blackPawns & Rank_7) >> 16;
}
