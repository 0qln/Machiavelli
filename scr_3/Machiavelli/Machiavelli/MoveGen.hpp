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


	void GeneratePseudoLegalMoves(Move* moveList, Color us) {
		auto pieces = _board->GetColorBitboard(us);
		
		auto square = Square(-1);
		auto pieceIdx = 0;
		while ((square = PopLsbIdx(&pieces)) != 64) {
			pieceIdx += GeneratePseudoLegalMoves(moveList+pieceIdx, square); 
		}
	}

	int GeneratePseudoLegalMoves(Move* moveList, const Square idx) {
		int moveCount = 0;
		Bitboard moves = GeneratePseudoLegalMoves(idx);
		Square square = -1;
		while ((square = PopLsbIdx(&moves)) != 64) {
			MoveHelper::Flag flag = MoveHelper::QUIET_MOVE_FLAG;
			*moveList++ = MoveHelper::Create(idx, square, flag);
			moveCount++;
		}
		return moveCount; 
	}

	Bitboard GeneratePseudoLegalMoves(const Square idx) {
		PieceType type = PieceType((_board->GetPiece(idx) & Piece::TypeMask) >> 1);
		switch (type)
		{
		default: 
		case PT_NULL: return Bitboard(0);
		case Pawn: return GeneratePawnMoves(idx);
		case Knight: return GenerateKnightMoves(idx);
		case Bishop: return GenerateBishopMoves(idx);
		case Rook: return GenerateRookMoves(idx);
		case Queen: return GenerateQueenMoves(idx);
		case King: return GenerateKingMoves(idx);
		}
	}

	Bitboard GenerateRookMoves(const Square rookIdx) {
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

	Bitboard GenerateBishopMoves(const Square bishopIdx) {
		Color us = Color(_board->GetPiece(bishopIdx) & Piece::ColorMask);
		Color nus = Color(!us); 

		Bitboard result = 0LL; 
		auto rankIdx = bishopIdx / 8;
		auto fileIdx = bishopIdx % 8;
		Bitboard self = 1ULL << bishopIdx;

		auto diag1Idx = 7 - rankIdx + fileIdx; 
		Bitboard diag1 = DiagMask1[diag1Idx];

		auto diag2Idx =	rankIdx + fileIdx;		
		Bitboard diag2 = DiagMask2[diag2Idx];

		Bitboard bMask, wMask, square; 

		Bitboard selfDivisorDown = ~FromIndex(bishopIdx);
		DeactivateBit(&selfDivisorDown, bishopIdx);

		Bitboard selfDivisorUp = FromIndex(bishopIdx);
		DeactivateBit(&selfDivisorUp, bishopIdx);

		// -< Bottom Left >- 
		bMask = _board->GetColorBitboard(nus) & diag1 & selfDivisorDown; 
		wMask = (_board->GetColorBitboard(us) << 9) & (diag1 & selfDivisorDown | self);
		square = (0xFFFFFFFFFFFFFFFF << 63 - MsbIdx(bMask | wMask)) & diag1 & selfDivisorDown;
		result |= square;

		// -< Up Right >-
		bMask = _board->GetColorBitboard(nus) & diag1 & selfDivisorUp;
		wMask = (_board->GetColorBitboard(us) >> 9) & (diag1 & selfDivisorUp | self);
		square = (0xFFFFFFFFFFFFFFFF >> 63 - LsbIdx(bMask | wMask)) & diag1 & selfDivisorUp; 
		result |= square;

		// -< Bottom Right >-
		bMask = _board->GetColorBitboard(nus) & diag2 & selfDivisorDown;
		wMask = (_board->GetColorBitboard(us) << 7) & (diag2 & selfDivisorDown | self);
		square = (0xFFFFFFFFFFFFFFFF << 63 - MsbIdx(bMask | wMask)) & diag2 & selfDivisorDown;
		result |= square;

		// -< Up Left >-
		bMask = _board->GetColorBitboard(nus) & diag2 & selfDivisorUp;
		wMask = (_board->GetColorBitboard(us) >> 7) & (diag2 & selfDivisorUp | self); 
		square = (0xFFFFFFFFFFFFFFFF >> 63 - LsbIdx(bMask | wMask)) & diag2 & selfDivisorUp; 
		result |= square;

		return result;
	}

	Bitboard GenerateKingMoves(const Square kingIdx) {
		Color us = Color(_board->GetPiece(kingIdx) & Piece::ColorMask);
		Color nus = Color(!us);		
		Bitboard result = 0ULL;
		auto rankIdx = kingIdx / 8;
		auto fileIdx = kingIdx % 8;
		
		auto ranks = RankMask[rankIdx];
		if (rankIdx < 7) ranks |= RankMask[rankIdx + 1];
		if (rankIdx > 0) ranks |= RankMask[rankIdx - 1];

		auto files = FileMask[fileIdx]; 
		if (fileIdx < 7) files |= FileMask[fileIdx + 1];
		if (fileIdx > 0) files |= FileMask[fileIdx - 1];

		result = files & ranks; 

		// Exclude allies and the king himself
		result ^= _board->GetColorBitboard(us); 

		// King side castling
		if (_board->GetCastlingRights(us, true)) {
			result |= RankMask[rankIdx] & FileMask[FileTable::H];
		}

		// Queen side castling
		if (_board->GetCastlingRights(us, false)) {
			result |= RankMask[rankIdx] & FileMask[FileTable::A];
		}

		return result; 
	}

	Bitboard GenerateQueenMoves(const Square queenIdx) {
		Color us = Color(_board->GetPiece(queenIdx) & Piece::ColorMask);
		Color nus = Color(!us);

		return GenerateRookMoves(queenIdx) | GenerateBishopMoves(queenIdx);
	}

	Bitboard GenerateKnightMoves(const Square knightIdx) {
		Color us = Color(_board->GetPiece(knightIdx) & Piece::ColorMask); 
		Color nus = Color(!us); 
		Bitboard result = 0ULL;
		auto rankIdx = knightIdx / 8;
		auto fileIdx = knightIdx % 8;
		Bitboard self = 1ULL << knightIdx;
		
		// Include typical squares
		// upper
		if (rankIdx < 6) {
			// right
			if (fileIdx < 7)
				result |= self << 17; 
			// left
			if (fileIdx > 0)
				result |= self << 15;
		}
		// up
		if (rankIdx < 7) {
			// righter
			if (fileIdx < 6)
				result |= self << 10; 
			// lefter
			if (fileIdx > 1)
				result |= self << 6;
		}
		// lower
		if (rankIdx > 1) {
			// left
			if (fileIdx > 0) 
				result |= self >> 17; 
			// right
			if (fileIdx < 7)
				result |= self >> 15;
		}
		// low
		if (rankIdx > 0) {
			// lefter
			if (fileIdx > 1) 
				result |= self >> 10; 
			// righter
			if (fileIdx < 6)
				result |= self >> 6;
		}

		// Exclude squares where allies are standing on
		result ^= _board->GetColorBitboard(us) & result;

		return result;
	}

	Bitboard GeneratePawnMoves(const Square pawnIdx) {
		Color us = Color(_board->GetPiece(pawnIdx) & Piece::ColorMask);
		Color nus = Color(!us);

		Bitboard result = 0ULL;

		auto rankIdx = pawnIdx / 8;
		auto fileIdx = pawnIdx % 8;

		Bitboard enemies = _board->GetColorBitboard(nus);
		Bitboard friends = _board->GetColorBitboard(us);

		// Move 1 square forward
		// We don't need to check wether the pawn is on the last rank or not, which might cause overflow,
		// as the pawn can never be on the last rank. If it is moved there, it will have to promote.
		Bitboard forward1 = 1ULL << pawnIdx + (us == Color::White ? 8 : -8);
		if (!((enemies | friends) & forward1)) {
			result |= forward1;

			// Move 2 squares forward
			// If we can't move forward one square,
			// we won't be able to move forward two squares.
			//	=> This is nested inside the `forward1` condition 
			Bitboard forward2 = 1ULL << pawnIdx + (us == Color::White ? 16 : -16);
			if (rankIdx == (us == Color::White ? 1 : 6) && !((enemies | friends) & forward2)) {
				result |= forward2;
			}
		}

		// Capture left 
		Bitboard captureLeft = 1ULL << pawnIdx + (us == Color::White ? 7 : -7);
		if (fileIdx > 0 && enemies & captureLeft) { 
			result |= captureLeft; 
		}

		// Capture right
		Bitboard captureRight = 1ULL << pawnIdx + (us == Color::White ? 9 : -9);
		if (fileIdx < 7 && enemies & captureRight) { 
			result |= captureRight; 
		} 

		// En passant
		Bitboard enPassant = _board->GetEnPassant();
		if (rankIdx == (us == Color::White ? 4 : 3)) {
			if (fileIdx < 7 && (enPassant & FileMask[fileIdx + 1])) {
				result |= enPassant;
			}
			if (fileIdx > 0 && (enPassant & FileMask[fileIdx - 1])) {
				result |= enPassant; 
			}
		}

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