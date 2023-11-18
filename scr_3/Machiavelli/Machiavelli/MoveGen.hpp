#pragma once

#include <bit>
#include <bitset>
#include <cstdint>
#include <iostream>
#include <concepts>
#include <iomanip>

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


	MoveGen(Board *board) {
		_board = board;
	}

	Bitboard GenerateWhiteRookMoves(Square rookIdx)
	{
		Bitboard result = 0;
		int rankIdx = rookIdx / 8;
		int fileIdx = rookIdx % 8;
		Bitboard rank = RankMask[rankIdx];
		Bitboard file = FileMask[fileIdx];
		Bitboard rookSquare = 1ULL << rookIdx; 

		Bitboard selfDivisorDown = ~FromIndex(rookIdx); 
		DeactivateBit(&selfDivisorDown, rookIdx); 

		Bitboard selfDivisorUp = FromIndex(rookIdx);
		DeactivateBit(&selfDivisorUp, rookIdx); 

		Bitboard bMask, wMask, square;

		// -< Down >-
		bMask = _board->GetColorBitboard(Color::Black) & file &  selfDivisorDown;
		wMask = (_board->GetColorBitboard(Color::White) << 8) & (file & selfDivisorDown | rookSquare);
		square = (0xFFFFFFFFFFFFFFFF << 63 - MsbIdx(bMask | wMask)) & file & selfDivisorDown; 
		result |= square;

		// -< Up >- 
		bMask = _board->GetColorBitboard(Color::Black) & file & selfDivisorUp;
		wMask = (_board->GetColorBitboard(Color::White) >> 8) & (file & selfDivisorUp | rookSquare); 
		square = (0xFFFFFFFFFFFFFFFF >> 63 - LsbIdx(bMask | wMask)) & file & selfDivisorUp; 
		result |= square; 

		// -< Left >-
		bMask = _board->GetColorBitboard(Color::Black) & rank & selfDivisorDown;
		wMask = (_board->GetColorBitboard(Color::White) << 1) & (rank & selfDivisorDown | rookSquare);
		square = (0xFFFFFFFFFFFFFFFF << 63 - MsbIdx(bMask | wMask)) & rank & selfDivisorDown;  
		result |= square;

		// -< Right >-
		bMask = _board->GetColorBitboard(Color::Black) & rank & selfDivisorUp; 
		wMask = (_board->GetColorBitboard(Color::White) >> 1) & (rank & selfDivisorUp | rookSquare);
		square = (0xFFFFFFFFFFFFFFFF >> 63 - LsbIdx(bMask | wMask)) & rank & selfDivisorUp; 
		result |= square; 

		return result;
	}

	inline void DeactivateBit(Bitboard* board, const Square squareIdx) {
		*board &= ~(1ULL << squareIdx);
	}

	inline Bitboard FromIndex(Square idx) {
		return 0xFFFFFFFFFFFFFFFF << idx;
	}

	inline Bitboard Rotate90DegClockwise(Bitboard x) {
		return FlipVertical(FlipA1H8(x));
	}

	inline Bitboard Rotate90DegAntiClockwise(Bitboard x) {
		return FlipA8H1(FlipVertical(x));
	}

	inline Bitboard FlipA8H1(Bitboard x) {
		Bitboard t;
		const Bitboard k1 = 0x5500550055005500;
		const Bitboard k2 = 0x3333000033330000;
		const Bitboard k4 = 0x0f0f0f0f00000000;
		t = k4 & (x ^ (x << 28));
		x ^= t ^ (t >> 28);
		t = k2 & (x ^ (x << 14));
		x ^= t ^ (t >> 14);
		t = k1 & (x ^ (x << 7));
		x ^= t ^ (t >> 7);
		return x;
	}

	inline Bitboard FlipA1H8(Bitboard x) {
		Bitboard t;
		const Bitboard k1 = 0x5500550055005500;
		const Bitboard k2 = 0x3333000033330000;
		const Bitboard k4 = 0x0f0f0f0f00000000;
		t = k4 & (x ^ (x << 28));
		x ^= t ^ (t >> 28);
		t = k2 & (x ^ (x << 14));
		x ^= t ^ (t >> 14);
		t = k1 & (x ^ (x << 7));
		x ^= t ^ (t >> 7);
		return x;
	}

	inline Bitboard FlipVertical(Bitboard x) {
		return std::byteswap(x);
	}


	inline Square LsbIdx(Bitboard board) {
		return board ? std::countr_zero(board) : 63;
	}
	inline Square PopLsbIdx(Bitboard *board) {
		Square lsb = std::countr_zero(*board);
		*board &= *board - 1;
		return lsb;
	}
	inline Square MsbIdx(Bitboard board) {
		return board ? std::countl_zero(board) : 63;
	}
};