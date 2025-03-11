//
// Created by Eyeve on 10.03.2025.
//

#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <cinttypes>
#include <vector>

#include "types.h"
#include "move.h"


namespace Leslie
{
	using bitboard = uint64_t;
	using counter = uint8_t;
	using size_type = size_t;

    constexpr bitboard Rank1 = 0xFFull;
	constexpr bitboard Rank2 = Rank1 << (8 * 1);
	constexpr bitboard Rank3 = Rank1 << (8 * 2);
	constexpr bitboard Rank4 = Rank1 << (8 * 3);
	constexpr bitboard Rank5 = Rank1 << (8 * 4);
	constexpr bitboard Rank6 = Rank1 << (8 * 5);
	constexpr bitboard Rank7 = Rank1 << (8 * 6);
	constexpr bitboard Rank8 = Rank1 << (8 * 7);

	constexpr bitboard FileA = 0x8080808080808080ull;
	constexpr bitboard FileB = FileA >> 1;
	constexpr bitboard FileC = FileA >> 2;
	constexpr bitboard FileD = FileA >> 3;
	constexpr bitboard FileE = FileA >> 4;
	constexpr bitboard FileF = FileA >> 5;
	constexpr bitboard FileG = FileA >> 6;
	constexpr bitboard FileH = FileA >> 7;

	std::string to_str(bitboard board);

	class Board {

	  public:
		Board();

		Board make_moves(Move* moves, size_type size);
		void get_moves(std::vector<Move>& vec);

	  private:
		bitboard kings[2];
		bitboard queens[2];
		bitboard rooks[2];
		bitboard bishops[2];
		bitboard knights[2];
		bitboard pawns[2];

		bitboard enPassantCapture;
		counter rule50;
		counter movesCount;

		Color turn;
		bool whiteKingCastle;
		bool whiteQueenCastle;
		bool blackKingCastle;
		bool blackQueenCastle;

		void add_piece_moves(void (Board::* adder)(bitboard, std::vector<Move>&), bitboard pieces, std::vector<Move>& vec);
		void add_king_moves(bitboard position, std::vector<Move>& vec);
		void add_queen_moves(bitboard position, std::vector<Move>& vec);
		void add_rook_moves(bitboard position, std::vector<Move>& vec);
		void add_bishop_moves(bitboard position, std::vector<Move>& vec);
		void add_knight_moves(bitboard position, std::vector<Move>& vec);
		void add_white_pawn_moves(bitboard position, std::vector<Move>& vec);
		void add_black_pawn_moves(bitboard position, std::vector<Move>& vec);

		inline static bitboard up_one(bitboard board) { return board << 8; }
		inline static bitboard down_one(bitboard board) { return board >> 8; }

	};

}

#endif //BOARD_H
