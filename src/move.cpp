#include "move.h"


namespace Leslie
{
	Move::Move(PieceType type, bitboard from, bitboard to): type(type), from(from), to(to)
	{

	}

	std::size_t MoveHash::operator()(const Move& obj) const
	{
		std::size_t hash = 0;
		hash ^= std::hash<int>()(obj.type) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
		hash ^= std::hash<bitboard>()(obj.from) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
		hash ^= std::hash<bitboard>()(obj.to) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
		return hash;
	}
}
