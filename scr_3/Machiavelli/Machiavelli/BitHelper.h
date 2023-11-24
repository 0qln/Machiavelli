#pragma once

#include <bit>
#include <bitset>
#include <cstdint>
#include <iostream>
#include <concepts>
#include <iomanip>

#include "Misc.h"

class BitHelper {
public:

	static inline void DeactivateBit(Bitboard* board, const Square* squareIdx) {
		*board &= ~(1ULL << *squareIdx);
	}
	
	static inline void ActivateBit(Bitboard* board, const Square* squareIdx) {
		*board |= (1ULL << *squareIdx);
	}
	
	static inline void FlipBit(Bitboard* board, const Square* squareIdx) {
		*board ^= (1ULL << *squareIdx);
	}
	
	static inline void FlipBits(Bitboard* board, const Square* squareIdx1, const Square* squareIdx2) {
		*board ^= (1ULL << *squareIdx1 | 1ULL << *squareIdx2);
	}
	
	static inline bool GetBit(Bitboard board, const Square* squareIdx) {
		return (board >> *squareIdx) & 1;
	}

	static inline void SetBit(Bitboard* board, const Square* squareIdx, const bool value) {
		*board = (*board & (1ULL << *squareIdx)) | *board << *squareIdx;
	}

	static inline int CountBits(Bitboard bb) {
		int result = 0;
		while (PopLsbIdx(&bb) != 64) {
			++result;
		}
		return result;
	}

	static inline Bitboard FromIndex(Square idx) {
		return 0xFFFFFFFFFFFFFFFF << idx;
	}

	static inline Bitboard Rotate90DegClockwise(Bitboard x) {
		return FlipVertical(FlipA1H8(x));
	}

	static inline Bitboard Rotate90DegAntiClockwise(Bitboard x) {
		return FlipA8H1(FlipVertical(x));
	}

	static inline Bitboard FlipA8H1(Bitboard x) {
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

	static inline Bitboard FlipA1H8(Bitboard x) {
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

	static inline Bitboard FlipVertical(Bitboard x) {
		return std::byteswap(x);
	}


	static inline Square LsbIdx(Bitboard board) {
		return board ? std::countr_zero(board) : 63;
	}
	static inline Square PopLsbIdx(Bitboard* board) {
		Square lsb = std::countr_zero(*board);
		*board &= *board - 1;
		return lsb;
	}
	static inline Square MsbIdx(Bitboard board) {
		return board ? std::countl_zero(board) : 63;
	}
};