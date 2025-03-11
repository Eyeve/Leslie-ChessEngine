//
// Created by Eyeve on 10.03.2025.
//

#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <inttypes.h>


namespace Leslie
{
	using bitboard = uint64_t;
	using counter = int16_t;

    constexpr bitboard Rank1 = 0xFF;
	constexpr bitboard Rank2 = Rank1 << (8 * 1);
	constexpr bitboard Rank3 = Rank1 << (8 * 2);
	constexpr bitboard Rank4 = Rank1 << (8 * 3);
	constexpr bitboard Rank5 = Rank1 << (8 * 4);
	constexpr bitboard Rank6 = Rank1 << (8 * 5);
	constexpr bitboard Rank7 = Rank1 << (8 * 6);
	constexpr bitboard Rank8 = Rank1 << (8 * 7);

	constexpr bitboard FileA = 0x8080808080808080;
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

		inline bitboard get_white_king_moves() { return get_white_king_advances() | get_white_king_attacks(); }
		inline bitboard get_white_queen_moves() { return get_white_queen_advances() | get_white_queen_attacks(); }
		inline bitboard get_white_rook_moves() { return get_white_rook_advances() | get_white_rook_attacks(); }
		inline bitboard get_white_bishop_moves() { return get_white_bishop_advances() | get_white_bishop_attacks(); }
		inline bitboard get_white_knight_moves() { return get_white_knight_advances() | get_white_knight_attacks(); }
		inline bitboard get_white_pawn_moves() { return get_white_pawn_advances() | get_white_pawn_attacks(); }

		inline bitboard get_black_king_moves() { return get_black_king_advances() | get_black_king_attacks(); }
		inline bitboard get_black_queen_moves() { return get_black_queen_advances() | get_black_queen_attacks(); }
		inline bitboard get_black_rook_moves() { return get_black_rook_advances() | get_black_rook_attacks(); }
		inline bitboard get_black_bishop_moves() { return get_black_bishop_advances() | get_black_bishop_attacks(); }
		inline bitboard get_black_knight_moves() { return get_black_knight_advances() | get_black_knight_attacks(); }
		inline bitboard get_black_pawn_moves() { return get_black_pawn_advances() | get_black_pawn_attacks(); }

		inline bitboard get_white_king_advances() { return get_king_advances(whiteKings); }
		inline bitboard get_white_queen_advances() { return get_queen_advances(whiteQueens); }
		inline bitboard get_white_rook_advances() { return get_rook_advances(whiteRooks); }
		inline bitboard get_white_bishop_advances() { return get_bishop_advances(whiteBishops); }
		inline bitboard get_white_knight_advances() { return get_knight_advances(whiteKnights); }
		bitboard get_white_pawn_advances();

		inline bitboard get_white_king_attacks() { return get_king_attacks(whiteKings); }
		inline bitboard get_white_queen_attacks() { return get_queen_attacks(whiteQueens); }
		inline bitboard get_white_rook_attacks() { return get_rook_attacks(whiteRooks); }
		inline bitboard get_white_bishop_attacks() { return get_bishop_attacks(whiteBishops); }
		inline bitboard get_white_knight_attacks() { return get_knight_attacks(whiteKnights); }
		bitboard get_white_pawn_attacks();

		inline bitboard get_black_king_advances() { return get_king_advances(blackKings); }
		inline bitboard get_black_queen_advances() { return get_queen_advances(blackQueens); }
		inline bitboard get_black_rook_advances() { return get_rook_advances(blackRooks); }
		inline bitboard get_black_bishop_advances() { return get_bishop_advances(blackBishops); }
		inline bitboard get_black_knight_advances() { return get_knight_advances(blackKnights); }
		bitboard get_black_pawn_advances();

		inline bitboard get_black_king_attacks() { return get_king_attacks(blackKings); }
		inline bitboard get_black_queen_attacks() { return get_queen_attacks(blackQueens); }
		inline bitboard get_black_rook_attacks() { return get_rook_attacks(blackRooks); }
		inline bitboard get_black_bishop_attacks() { return get_bishop_attacks(blackBishops); }
		inline bitboard get_black_knight_attacks() { return get_knight_attacks(blackKnights); }
		bitboard get_black_pawn_attacks();

	  private:
		bitboard whiteKings;
		bitboard whiteQueens;
		bitboard whiteRooks;
		bitboard whiteBishops;
		bitboard whiteKnights;
		bitboard whitePawns;

		bitboard blackKings;
		bitboard blackQueens;
		bitboard blackRooks;
		bitboard blackBishops;
		bitboard blackKnights;
		bitboard blackPawns;

		bitboard enPassantCapture;
		counter halfMovesCount;
		counter movesCount;

		bool isWhiteTurn;
		bool whiteKingCastle;
		bool whiteQueenCastle;
		bool blackKingCastle;
		bool blackQueenCastle;

		static bitboard get_king_advances(bitboard kings);
		static bitboard get_queen_advances(bitboard queens);
		static bitboard get_rook_advances(bitboard rooks);
		static bitboard get_bishop_advances(bitboard bishops);
		static bitboard get_knight_advances(bitboard knights);

		static bitboard get_king_attacks(bitboard kings);
		static bitboard get_queen_attacks(bitboard queens);
		static bitboard get_rook_attacks(bitboard rooks);
		static bitboard get_bishop_attacks(bitboard bishops);
		static bitboard get_knight_attacks(bitboard knights);

		inline static bitboard up_one(bitboard board) { return board << 8; }
		inline static bitboard down_one(bitboard board) { return board >> 8; }

	};

}


#endif //BOARD_H
