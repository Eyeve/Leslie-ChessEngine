#include "board.h"

#include <bit>

namespace Leslie
{
	std::string to_str(bitboard board)
	{
		char result[72];
		char *ptr = result;

		for (int rank = 7; rank >= 0; --rank)
		{
			for (int file = 7; file >= 0; --file)
			{
				*ptr++ = (board & (1ULL << (rank * 8 + file))) ? '1' : '0';
			}
			*ptr++ = '\n';
		}
		*ptr = '\0';
		return std::string{ result };
	}

	Board::Board(std::string fen)
	{

	}


	void Board::get_moves(std::vector< Move > &vec)
	{
		vec.clear();

		add_moves(&Board::add_king_moves, kings[turn], vec);
		add_moves(&Board::add_queen_moves, queens[turn], vec);
		add_moves(&Board::add_rook_moves, rooks[turn], vec);
		add_moves(&Board::add_bishop_moves, bishops[turn], vec);
		add_moves(&Board::add_knight_moves, knights[turn], vec);
		if (turn == Color::WHITE)
			add_moves(&Board::add_white_pawn_moves, pawns[turn], vec);
		else
			add_moves(&Board::add_black_pawn_moves, pawns[turn], vec);
	}

	void Board::add_moves(void (Board::*adder)(bitboard, std::vector< Move > &), bitboard pieces, std::vector< Move > &vec)
	{
		while (pieces)
		{
			bitboard position = 1ull << std::countr_zero(pieces);
			(this->*adder)(position, vec);
			pieces ^= position;
		}
	}

	void Board::add_piece_moves(bitboard from, bitboard to, PieceType type, std::vector< Move >& vec)
	{
		while (to)
		{
			bitboard position = 1ull << std::countr_zero(to);
			vec.emplace_back(type, turn, from, position, PieceType::NONE);
			to ^= position;
		}
	}

	void Board::add_king_moves(bitboard position, std::vector< Move > &vec)
	{
		bitboard result = (((position << 7) | (position >> 9) | (position >> 1)) & (~Leslie::FileH)) |
			(((position >> 7) | (position << 9) | (position << 1)) & (~Leslie::FileA)) |
			((position >> 8) | (position << 8));
		add_piece_moves(position, result, PieceType::KING, vec);
	}

	void Board::add_queen_moves(bitboard position, std::vector< Move > &vec) {}

	void Board::add_rook_moves(bitboard position, std::vector< Move > &vec) {}

	void Board::add_bishop_moves(bitboard position, std::vector< Move > &vec) {}

	void Board::add_knight_moves(bitboard position, std::vector< Move > &vec)
	{
		bitboard l1 = (position << 1) & ~Leslie::FileH;
		bitboard l2 = (position << 2) & ~(Leslie::FileH & Leslie::FileG);
		bitboard r1 = (position >> 1) & ~Leslie::FileA;
		bitboard r2 = (position >> 2) & ~(Leslie::FileA & Leslie::FileB);
		bitboard result = (l1 | r1) << 16 | (l1 | r1) >> 16 | (l2 | r2) << 8 | (l2 | r2) >> 8;
		add_piece_moves(position, result, PieceType::KNIGHT, vec);
	}

	void Board::add_white_pawn_moves(bitboard position, std::vector< Move > &vec) {}

	void Board::add_black_pawn_moves(bitboard position, std::vector< Move > &vec) {}

	// bitboard Board::get_white_pawn_advances()
	//    {
	//        return (whitePawns << 8) | (whitePawns & Rank2) << 16;
	//    }
	//
	//    bitboard Board::get_black_pawn_advances()
	//    {
	//        return (blackPawns >> 8) | (blackPawns & Rank7) >> 16;
	//    }
}	 // namespace Leslie
