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


public:
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

	// Diagonals parallel to (A1 to H8)
	const Bitboard DiagMask1[15] {
		0x8040201008040201ULL / 128 & (RankMask[7]),
		0x8040201008040201ULL / 64	& (RankMask[6] | RankMask[7]),
		0x8040201008040201ULL / 32	& (RankMask[5] | RankMask[6] | RankMask[7]),
		0x8040201008040201ULL / 16	& (RankMask[4] | RankMask[5] | RankMask[6] | RankMask[7]),
		0x8040201008040201ULL / 8	& (RankMask[3] | RankMask[4] | RankMask[5] | RankMask[6] | RankMask[7]),
		0x8040201008040201ULL / 4	& (RankMask[2] | RankMask[3] | RankMask[4] | RankMask[5] | RankMask[6] | RankMask[7]),
		0x8040201008040201ULL / 2	& (RankMask[1] | RankMask[2] | RankMask[3] | RankMask[4] | RankMask[5] | RankMask[6] | RankMask[7]),
		0x8040201008040201ULL,
		0x8040201008040201ULL * 2	& (FileMask[1] | FileMask[2] | FileMask[3] | FileMask[4] | FileMask[5] | FileMask[6] | FileMask[7]),
		0x8040201008040201ULL * 4	& (FileMask[2] | FileMask[3] | FileMask[4] | FileMask[5] | FileMask[6] | FileMask[7]),
		0x8040201008040201ULL * 8	& (FileMask[3] | FileMask[4] | FileMask[5] | FileMask[6] | FileMask[7]),
		0x8040201008040201ULL * 16	& (FileMask[4] | FileMask[5] | FileMask[6] | FileMask[7]),
		0x8040201008040201ULL * 32	& (FileMask[5] | FileMask[6] | FileMask[7]),
		0x8040201008040201ULL * 64	& (FileMask[6] | FileMask[7]),
		0x8040201008040201ULL * 128 & (FileMask[7]),
	};

	// Diagonals parallel to (A8 to H1)
	const Bitboard DiagMask2[15]{
		0x0102040810204080ULL / 128 & (FileMask[0]),
		0x0102040810204080ULL / 64	& (FileMask[1] | FileMask[0]),
		0x0102040810204080ULL / 32	& (FileMask[2] | FileMask[1] | FileMask[0]),
		0x0102040810204080ULL / 16	& (FileMask[3] | FileMask[2] | FileMask[1] | FileMask[0]),
		0x0102040810204080ULL / 8	& (FileMask[4] | FileMask[3] | FileMask[2] | FileMask[1] | FileMask[0]),
		0x0102040810204080ULL / 4	& (FileMask[5] | FileMask[4] | FileMask[3] | FileMask[2] | FileMask[1] | FileMask[0]),
		0x0102040810204080ULL / 2	& (FileMask[6] | FileMask[5] | FileMask[4] | FileMask[3] | FileMask[2] | FileMask[1] | FileMask[0]),
		0x0102040810204080ULL,
		0x0102040810204080ULL * 2	& (FileMask[7] | FileMask[6] | FileMask[5] | FileMask[4] | FileMask[3] | FileMask[2] | FileMask[1]),
		0x0102040810204080ULL * 4	& (FileMask[7] | FileMask[6] | FileMask[5] | FileMask[4] | FileMask[3] | FileMask[2]),
		0x0102040810204080ULL * 8	& (FileMask[7] | FileMask[6] | FileMask[5] | FileMask[4] | FileMask[3]),
		0x0102040810204080ULL * 16	& (FileMask[7] | FileMask[6] | FileMask[5] | FileMask[4]),
		0x0102040810204080ULL * 32	& (FileMask[7] | FileMask[6] | FileMask[5]),
		0x0102040810204080ULL * 64	& (FileMask[7] | FileMask[6]),
		0x0102040810204080ULL * 128 & (FileMask[7]),
	};


	MoveGen(Board *board) {
		_board = board;
	}


	Bitboard GenerateRookMoves(Square rookIdx)
	{
		Color us = Color(_board->GetPiece(rookIdx) & Piece::ColorMask);
		Color nus = Color(!us);

		Bitboard result = 0;
		Bitboard rank = RankMask[rookIdx / 8];
		Bitboard file = FileMask[rookIdx % 8];
		Bitboard rookSquare = 1ULL << rookIdx;

		Bitboard selfDivisorDown = ~FromIndex(rookIdx);
		DeactivateBit(&selfDivisorDown, rookIdx);

		Bitboard selfDivisorUp = FromIndex(rookIdx);
		DeactivateBit(&selfDivisorUp, rookIdx);

		Bitboard bMask, wMask, square;

		// -< Down >-
		bMask = _board->GetColorBitboard(nus) & file & selfDivisorDown;
		wMask = (_board->GetColorBitboard(us) << 8) & (file & selfDivisorDown | rookSquare); 
		square = (0xFFFFFFFFFFFFFFFF << 63 - MsbIdx(bMask | wMask)) & file & selfDivisorDown;
		result |= square;

		// -< Up >- 
		bMask = _board->GetColorBitboard(nus) & file & selfDivisorUp; 
		wMask = (_board->GetColorBitboard(us) >> 8) & (file & selfDivisorUp | rookSquare); 
		square = (0xFFFFFFFFFFFFFFFF >> 63 - LsbIdx(bMask | wMask)) & file & selfDivisorUp;
		result |= square;

		// -< Left >-
		bMask = _board->GetColorBitboard(nus) & rank & selfDivisorDown; 
		wMask = (_board->GetColorBitboard(us) << 1) & (rank & selfDivisorDown | rookSquare); 
		square = (0xFFFFFFFFFFFFFFFF << 63 - MsbIdx(bMask | wMask)) & rank & selfDivisorDown;
		result |= square;

		// -< Right >-
		bMask = _board->GetColorBitboard(nus) & rank & selfDivisorUp; 
		wMask = (_board->GetColorBitboard(us) >> 1) & (rank & selfDivisorUp | rookSquare); 
		square = (0xFFFFFFFFFFFFFFFF >> 63 - LsbIdx(bMask | wMask)) & rank & selfDivisorUp;
		result |= square;

		return result;
	}


	Bitboard GenerateBishopMoves(Square bishopIdx) {
		Bitboard diag1 = DiagMask1[0];
		Bitboard diag2 = DiagMask2[0];

		return diag2 /*| diag2*/;
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