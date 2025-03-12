#include "position.h"

#include <bit>
#include <sstream>


namespace Leslie
{
	std::string to_str(const bitboard board)
	{
		char str[Board::StringFormatSize];
		char *ptr = str;

		for (bitboard i = Board::start(); Board::is_end(i); i = Board::next(i))
		{
			*ptr++ = board & i ? '1' : '0';
			if (Board::is_endl(i))
				*ptr++ = '\n';
		}
		*ptr = '\0';
		return std::string{ str };
	}

	Position::Position(std::string fen) :
		kings{ 0ull, 0ull }, queens{ 0ull, 0ull }, rooks{ 0ull, 0ull }, bishops{ 0ull, 0ull }, knights{ 0ull, 0ull },
		pawns{ 0ull, 0ull }, enPassantCapture{ 0ull }, rule50{ 0 }, movesCount{ 0 }, turn{ Color::WHITE },
		whiteKingCastle{ false }, whiteQueenCastle{ false }, blackKingCastle{ false }, blackQueenCastle{ false }
	{
		std::istringstream iss(fen);
		std::string boardPart, turnPart, castlingPart, enPassantPart;

		iss >> boardPart >> turnPart >> castlingPart >> enPassantPart >> rule50 >> movesCount;

		bitboard i = Board::start();
		for (char c : boardPart)
		{
			if (c == '/')
				continue;

			if (isdigit(c))
			{
				i = Board::shift(i, c - '0');
				continue;
			}

			bitboard *pieceBitboards = get_piece_bitboards(c);
			Color color = isupper(c) ? WHITE : BLACK;

			if (pieceBitboards != nullptr)
				pieceBitboards[color] |= i;
			i = Board::next(i);
		}

		turn = (turnPart == "w") ? WHITE : BLACK;

		whiteKingCastle = castlingPart.find('K') != std::string::npos;
		whiteQueenCastle = castlingPart.find('Q') != std::string::npos;
		blackKingCastle = castlingPart.find('k') != std::string::npos;
		blackQueenCastle = castlingPart.find('q') != std::string::npos;

		if (enPassantPart != "-")
		{
			int epFile = enPassantPart[0] - 'a';
			int epRank = enPassantPart[1] - '1';
			enPassantCapture = (1ULL << (epRank * 8 + epFile));
		}
	}

	bitboard* Position::get_piece_bitboards(char c)
	{
		switch (tolower(c))
		{
		case 'k': return kings;
		case 'q': return queens;
		case 'r': return rooks;
		case 'b': return bishops;
		case 'n': return knights;
		case 'p': return pawns;
		default:
			// TODO: add debug output message
			return nullptr;
		}
	}


	char Position::get_piece_char(bitboard location) const
	{
		if (kings[WHITE] & location) return 'K';
		if (kings[BLACK] & location) return 'k';
		if (queens[WHITE] & location) return 'Q';
		if (queens[BLACK] & location) return 'q';
		if (rooks[WHITE] & location) return 'R';
		if (rooks[BLACK] & location) return 'r';
		if (bishops[WHITE] & location) return 'B';
		if (bishops[BLACK] & location) return 'b';
		if (knights[WHITE] & location) return 'N';
		if (knights[BLACK] & location) return 'n';
		if (pawns[WHITE] & location) return 'P';
		if (pawns[BLACK] & location) return 'p';
		return '.';
	}

	Position::operator std::string() const
	{
		char str[Board::StringFormatSize];
		char *ptr = str;

		for (bitboard i = Board::start(); Board::is_end(i); i = Board::next(i)) {
			*ptr++ = get_piece_char(i);
			if (Board::is_endl(i))
				*ptr++ = '\n';
		}
		*ptr = '\0';
		return std::string{ str };
	}

	Color Position::get_opponent() const
	{
		return static_cast<Color>(static_cast<int>(turn) ^ 0b1);
	}

	bitboard Position::get_blockers(Color color) const
	{
		return queens[color] | rooks[color] | bishops[color] | pawns[color];
	}

	void Position::get_moves(std::vector< Move > &vec) const
	{
		vec.clear();

		add_moves(&Position::add_king_moves, kings[turn], vec);
		add_moves(&Position::add_queen_moves, queens[turn], vec);
		add_moves(&Position::add_rook_moves, rooks[turn], vec);
		add_moves(&Position::add_bishop_moves, bishops[turn], vec);
		add_moves(&Position::add_knight_moves, knights[turn], vec);
		if (turn == Color::WHITE)
			add_moves(&Position::add_white_pawn_moves, pawns[turn], vec);
		else
			add_moves(&Position::add_black_pawn_moves, pawns[turn], vec);
	}

	void Position::add_moves(void (Position::*adder)(bitboard, std::vector< Move > &) const, bitboard pieces, std::vector< Move > &vec) const
	{
		while (pieces)
		{
			bitboard position = 1ull << std::countr_zero(pieces);
			(this->*adder)(position, vec);
			pieces ^= position;
		}
	}

	void Position::add_piece_moves(bitboard from, bitboard to, PieceType type, std::vector< Move > &vec) const
	{
		while (to)
		{
			bitboard position = 1ull << std::countr_zero(to);
			vec.emplace_back(type, from, position);
			to ^= position;
		}
	}

	void Position::add_king_moves(bitboard position, std::vector< Move > &vec) const
	{
		bitboard a = (((position << 7) | (position >> 9) | (position >> 1)) & (~Leslie::FileA));
		bitboard b = (((position >> 7) | (position << 9) | (position << 1)) & (~Leslie::FileH));
		bitboard c = ((position >> 8) | (position << 8));
		bitboard result = a | b | c;
		add_piece_moves(position, result, PieceType::KING, vec);
	}

	void Position::add_queen_moves(bitboard position, std::vector< Move > &vec) const {}

	void Position::add_rook_moves(bitboard position, std::vector< Move > &vec) const
	{
		// add_piece_moves(position, result, PieceType::ROOK, vec);
	}

	void Position::add_bishop_moves(bitboard position, std::vector< Move > &vec) const
	{
		// add_piece_moves(position, result, PieceType::BISHOP, vec);
	}

	void Position::add_knight_moves(bitboard position, std::vector< Move > &vec) const
	{
		bitboard l1 = (position >> 1) & ~Leslie::FileA;
		bitboard l2 = (position >> 2) & ~(Leslie::FileA | Leslie::FileB);
		bitboard r1 = (position << 1) & ~Leslie::FileH;
		bitboard r2 = (position << 2) & ~(Leslie::FileH | Leslie::FileG);
		bitboard result = (l1 | r1) << 16 | (l1 | r1) >> 16 | (l2 | r2) << 8 | (l2 | r2) >> 8;
		add_piece_moves(position, result, PieceType::KNIGHT, vec);
	}

	void Position::add_white_pawn_moves(bitboard position, std::vector< Move > &vec) const
	{
		bitboard white_pawns = pawns[Color::WHITE];
		bitboard short_moves = white_pawns << 8;
		bitboard long_moves = (white_pawns & Rank2) << 16;
		bitboard attacks = get_blockers(get_opponent()) & (((short_moves << 1) & ~FileH) | ((short_moves >> 1) & ~FileA));
		bitboard result = short_moves | long_moves | attacks;

		add_piece_moves(position, result, PieceType::PAWN, vec);
	}

	void Position::add_black_pawn_moves(bitboard position, std::vector< Move > &vec) const
	{
		bitboard black_pawns = pawns[Color::BLACK];
		bitboard short_moves = black_pawns >> 8;
		bitboard long_moves = (black_pawns & Rank7) >> 16;
		bitboard attacks = get_blockers(get_opponent()) & (((short_moves << 1) & ~FileH) | ((short_moves >> 1) & ~FileA));
		bitboard result = short_moves | long_moves | attacks;

		add_piece_moves(position, result, PieceType::PAWN, vec);
	}
}
