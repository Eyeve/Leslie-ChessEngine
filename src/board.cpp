#include "board.h"

#include <bit>
#include <sstream>

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

	Board::Board(std::string fen) :
		kings{ 0ull, 0ull }, queens{ 0ull, 0ull }, rooks{ 0ull, 0ull }, bishops{ 0ull, 0ull }, knights{ 0ull, 0ull },
		pawns{ 0ull, 0ull }, enPassantCapture{ 0ull }, rule50{ 0 }, movesCount{ 0 }, turn{ Color::WHITE },
		whiteKingCastle{ false }, whiteQueenCastle{ false }, blackKingCastle{ false }, blackQueenCastle{ false }
	{
		std::istringstream iss(fen);
		std::string boardPart, turnPart, castlingPart, enPassantPart;

		iss >> boardPart >> turnPart >> castlingPart >> enPassantPart >> rule50 >> movesCount;

		int rank = 7, file = 0;
		for (char c : boardPart)
		{
			if (c == '/')
			{
				--rank;
				file = 0;
			}
			else if (isdigit(c))
			{
				file += c - '0';
			}
			else
			{
				bitboard *pieceArray = nullptr;
				Color color = isupper(c) ? WHITE : BLACK;

				switch (tolower(c))
				{
				case 'k':
					pieceArray = kings;
					break;
				case 'q':
					pieceArray = queens;
					break;
				case 'r':
					pieceArray = rooks;
					break;
				case 'b':
					pieceArray = bishops;
					break;
				case 'n':
					pieceArray = knights;
					break;
				case 'p':
					pieceArray = pawns;
					break;
				}

				if (pieceArray)
					pieceArray[color] |= (1ULL << (rank * 8 + 7 - file));

				file++;
			}
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

	std::string Board::to_str() {
		char result[72];
		char *ptr = result;

		for (int rank = 7; rank >= 0; --rank) {
			for (int file = 0; file < 8; ++file) {
				bitboard pos = 1ULL << (rank * 8 + 7 - file);
				char piece = '.';

				if (kings[WHITE] & pos) piece = 'K';
				else if (kings[BLACK] & pos) piece = 'k';
				else if (queens[WHITE] & pos) piece = 'Q';
				else if (queens[BLACK] & pos) piece = 'q';
				else if (rooks[WHITE] & pos) piece = 'R';
				else if (rooks[BLACK] & pos) piece = 'r';
				else if (bishops[WHITE] & pos) piece = 'B';
				else if (bishops[BLACK] & pos) piece = 'b';
				else if (knights[WHITE] & pos) piece = 'N';
				else if (knights[BLACK] & pos) piece = 'n';
				else if (pawns[WHITE] & pos) piece = 'P';
				else if (pawns[BLACK] & pos) piece = 'p';

				*ptr++ = piece;
			}
			*ptr++ = '\n';
		}
		*ptr = '\0';
		return std::string{ result };
	}

	bitboard Board::get_blockers()
	{
		const auto opponentColor = static_cast<Color>(~static_cast<int>(turn) & 0b11);
		return queens[opponentColor] | rooks[opponentColor] | bishops[opponentColor] | knights[opponentColor] | pawns[opponentColor];
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

	void Board::add_piece_moves(bitboard from, bitboard to, PieceType type, std::vector< Move > &vec)
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
		bitboard a = (((position << 7) | (position >> 9) | (position >> 1)) & (~Leslie::FileH));
		bitboard b = (((position >> 7) | (position << 9) | (position << 1)) & (~Leslie::FileA));
		bitboard c = ((position >> 8) | (position << 8));
		bitboard result = a | b | c;
		add_piece_moves(position, result, PieceType::KING, vec);
	}

	void Board::add_queen_moves(bitboard position, std::vector< Move > &vec) {}

	void Board::add_rook_moves(bitboard position, std::vector< Move > &vec) {}

	void Board::add_bishop_moves(bitboard position, std::vector< Move > &vec) {}

	void Board::add_knight_moves(bitboard position, std::vector< Move > &vec)
	{
		bitboard l1 = (position >> 1) & ~Leslie::FileH;
		bitboard l2 = (position >> 2) & ~(Leslie::FileH & Leslie::FileG);
		bitboard r1 = (position << 1) & ~Leslie::FileA;
		bitboard r2 = (position << 2) & ~(Leslie::FileA & Leslie::FileB);
		bitboard result = (l1 | r1) << 16 | (l1 | r1) >> 16 | (l2 | r2) << 8 | (l2 | r2) >> 8;
		add_piece_moves(position, result, PieceType::KNIGHT, vec);
	}

	void Board::add_white_pawn_moves(bitboard position, std::vector< Move > &vec)
	{
		bitboard white_pawns = pawns[Color::WHITE];
		bitboard short_moves = white_pawns << 8;
		bitboard long_moves = (white_pawns & Rank2) << 16;
		bitboard attacks = get_blockers() & (((short_moves << 1) & ~FileH) | ((short_moves >> 1) & ~FileA));
		bitboard result = short_moves | long_moves | attacks;

		add_piece_moves(position, result, PieceType::PAWN, vec);
	}

	void Board::add_black_pawn_moves(bitboard position, std::vector< Move > &vec)
	{
		bitboard black_pawns = pawns[Color::BLACK];
		bitboard short_moves = black_pawns >> 8;
		bitboard long_moves = (black_pawns & Rank7) >> 16;
		bitboard attacks = get_blockers() & (((short_moves << 1) & ~FileH) | ((short_moves >> 1) & ~FileA));
		bitboard result = short_moves | long_moves | attacks;

		add_piece_moves(position, result, PieceType::PAWN, vec);
	}
}
