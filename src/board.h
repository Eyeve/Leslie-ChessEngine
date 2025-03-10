//
// Created by Eyeve on 10.03.2025.
//

#ifndef BOARD_H
#define BOARD_H

#include <inttypes.h>


class Board {

  public:
	using bitboard = uint64_t;
	using counter = int16_t;

	static constexpr bitboard Rank_2 = 0x000000000000FF00;
	static constexpr bitboard Rank_7 = 0x00FF000000000000;

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
	counter halfmovesCount;
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



#endif //BOARD_H
