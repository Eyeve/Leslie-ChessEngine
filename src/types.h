//
// Created by Eyeve on 11.03.2025.
//

#ifndef COLOR_H
#define COLOR_H

#include <cinttypes>


#define ROW(sq1, sq2, sq3, sq4, sq5, sq6, sq7, sq8, rank) \
    sq1 = Leslie::FileA & rank, \
    sq2 = sq1 >> 1, \
    sq3 = sq1 >> 2, \
    sq4 = sq1 >> 3, \
    sq5 = sq1 >> 4, \
    sq6 = sq1 >> 5, \
    sq7 = sq1 >> 6, \
    sq8 = sq1 >> 7

namespace Leslie
{
	using bitboard = uint64_t;
	using counter = uint8_t;
	using size_type = size_t;
	using blockers_mask = uint16_t;

	static constexpr auto StartFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

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

	enum Color
	{
		WHITE = 0,
		BLACK = 1,
	};

	enum PieceType
	{
		KING,
		QUEEN,
		ROOK,
		BISHOP,
		KNIGHT,
		PAWN,
	};

	enum Square: bitboard
	{
		ROW(A8, B8, C8, D8, E8, F8, G8, H8, Rank8),
		ROW(A7, B7, C7, D7, E7, F7, G7, H7, Rank7),
		ROW(A6, B6, C6, D6, E6, F6, G6, H6, Rank6),
		ROW(A5, B5, C5, D5, E5, F5, G5, H5, Rank5),
		ROW(A4, B4, C4, D4, E4, F4, G4, H4, Rank4),
		ROW(A3, B3, C3, D3, E3, F3, G3, H3, Rank3),
		ROW(A2, B2, C2, D2, E2, F2, G2, H2, Rank2),
		ROW(A1, B1, C1, D1, E1, F1, G1, H1, Rank1),
	};

	enum Direction
	{
		NORTH,
		EAST,
		SOUTH,
		WEST,
		NORTH_EAST,
		NORTH_WEST,
		SOUTH_EAST,
		SOUTH_WEST,
	};

	namespace Board
	{
		// 64 cells + 8 end lines + 1 \0 symbol
		constexpr size_type StringFormatSize = 64 + 8 + 1;

		inline bitboard start() { return static_cast<bitboard>(A8); }
		inline bitboard next(bitboard i) { return i >> 1; }
		inline bitboard shift(bitboard i) { return i >> std::countr_zero(i) % 8 + 1; }
		inline bitboard shift(bitboard i, int off) { return i >> off; }
		inline bool is_end(bitboard i) { return i > 0ull; }
		inline bool is_endl(bitboard i) { return std::countr_zero(i) % 8 == 0; }
	}
}




#endif //COLOR_H
