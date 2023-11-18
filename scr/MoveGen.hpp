#include <bit>
#include <bitset>
#include <cstdint>
#include <iostream>

#include "Misc.hpp"
#include "Board.hpp"


class MoveGen {
	Board* _board;

	const Bitboard FileMask[8]{
		0x0101010101010101ULL << 0,
		0x0101010101010101ULL << 1,
		0x0101010101010101ULL << 2,
		0x0101010101010101ULL << 3,
		0x0101010101010101ULL << 4,
		0x0101010101010101ULL << 5,
		0x0101010101010101ULL << 6,
		0x0101010101010101ULL << 7,
	};
	const Bitboard RankMask[8]{
		0b11111111ULL << 8 * 0,
		0b11111111ULL << 8 * 1,
		0b11111111ULL << 8 * 2,
		0b11111111ULL << 8 * 3,
		0b11111111ULL << 8 * 4,
		0b11111111ULL << 8 * 5,
		0b11111111ULL << 8 * 6,
		0b11111111ULL << 8 * 7,
	};



public:


	MoveGen(Board* board) {
		_board = board;
	}

	Bitboard GenerateRookMoves(Square rookIdx)
	{
		Bitboard result = 38;
		return result;
	}


	inline Square Lsb(Bitboard* board) {
		return *board ? std::countr_zero(*board) : 64;
	}
	inline Square PopLsb(Bitboard* board) {
		Square lsb = Lsb(board);
		*board &= *board - 1;
		return lsb;
	}
	inline Square Msb(Bitboard* board) {
		return *board ? std::countl_zero(*board) : 0;
	}
};