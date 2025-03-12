#ifndef BOARD_H
#define BOARD_H

#include "move.h"
#include "types.h"

#include <string>
#include <vector>


namespace Leslie
{
	std::string to_str(const bitboard board);

	class Position
	{
	  public:
		Position(std::string fen);
		Position(const Position& pos) = default;

		operator std::string() const;

		Position make_moves(Move* moves, size_type size) const;
		void get_moves(std::vector< Move >& vec) const;

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

        char get_piece_char(bitboard place) const;
		bitboard* get_piece_bitboards(char c);

		bitboard get_blockers() const;

		void add_moves(void (Position::*adder)(bitboard, std::vector< Move >&) const, bitboard pieces, std::vector< Move >& vec) const;
		void add_piece_moves(bitboard from, bitboard to, PieceType type, std::vector< Move >& vec) const;

		void add_king_moves(bitboard position, std::vector< Move >& vec) const;
		void add_queen_moves(bitboard position, std::vector< Move >& vec) const;
		void add_rook_moves(bitboard position, std::vector< Move >& vec) const;
		void add_bishop_moves(bitboard position, std::vector< Move >& vec) const;
		void add_knight_moves(bitboard position, std::vector< Move >& vec) const;
		void add_white_pawn_moves(bitboard position, std::vector< Move >& vec) const;
		void add_black_pawn_moves(bitboard position, std::vector< Move >& vec) const;

		inline static bitboard up_one(bitboard board) { return board << 8; }
		inline static bitboard down_one(bitboard board) { return board >> 8; }
	};

}

#endif
