#pragma once


#include <bit>
#include <bitset>
#include <cstdint>
#include <iostream>
#include <concepts>
#include <iomanip>

#include "Misc.h"
#include "Board.h"
#include "BitHelper.h"


class Board;


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
	const Bitboard DiagMask1[15]{
		0x8040201008040201ULL / 128 & (RankMask[7]),
		0x8040201008040201ULL / 64 & (RankMask[6] | RankMask[7]),
		0x8040201008040201ULL / 32 & (RankMask[5] | RankMask[6] | RankMask[7]),
		0x8040201008040201ULL / 16 & (RankMask[4] | RankMask[5] | RankMask[6] | RankMask[7]),
		0x8040201008040201ULL / 8 & (RankMask[3] | RankMask[4] | RankMask[5] | RankMask[6] | RankMask[7]),
		0x8040201008040201ULL / 4 & (RankMask[2] | RankMask[3] | RankMask[4] | RankMask[5] | RankMask[6] | RankMask[7]),
		0x8040201008040201ULL / 2 & (RankMask[1] | RankMask[2] | RankMask[3] | RankMask[4] | RankMask[5] | RankMask[6] | RankMask[7]),
		0x8040201008040201ULL,
		0x8040201008040201ULL * 2 & (FileMask[1] | FileMask[2] | FileMask[3] | FileMask[4] | FileMask[5] | FileMask[6] | FileMask[7]),
		0x8040201008040201ULL * 4 & (FileMask[2] | FileMask[3] | FileMask[4] | FileMask[5] | FileMask[6] | FileMask[7]),
		0x8040201008040201ULL * 8 & (FileMask[3] | FileMask[4] | FileMask[5] | FileMask[6] | FileMask[7]),
		0x8040201008040201ULL * 16 & (FileMask[4] | FileMask[5] | FileMask[6] | FileMask[7]),
		0x8040201008040201ULL * 32 & (FileMask[5] | FileMask[6] | FileMask[7]),
		0x8040201008040201ULL * 64 & (FileMask[6] | FileMask[7]),
		0x8040201008040201ULL * 128 & (FileMask[7]),
	};

	// Diagonals parallel to (A8 to H1)
	const Bitboard DiagMask2[15]{
		0x0102040810204080ULL / 128 & (FileMask[0]),
		0x0102040810204080ULL / 64 & (FileMask[1] | FileMask[0]),
		0x0102040810204080ULL / 32 & (FileMask[2] | FileMask[1] | FileMask[0]),
		0x0102040810204080ULL / 16 & (FileMask[3] | FileMask[2] | FileMask[1] | FileMask[0]),
		0x0102040810204080ULL / 8 & (FileMask[4] | FileMask[3] | FileMask[2] | FileMask[1] | FileMask[0]),
		0x0102040810204080ULL / 4 & (FileMask[5] | FileMask[4] | FileMask[3] | FileMask[2] | FileMask[1] | FileMask[0]),
		0x0102040810204080ULL / 2 & (FileMask[6] | FileMask[5] | FileMask[4] | FileMask[3] | FileMask[2] | FileMask[1] | FileMask[0]),
		0x0102040810204080ULL,
		0x0102040810204080ULL * 2 & (FileMask[7] | FileMask[6] | FileMask[5] | FileMask[4] | FileMask[3] | FileMask[2] | FileMask[1]),
		0x0102040810204080ULL * 4 & (FileMask[7] | FileMask[6] | FileMask[5] | FileMask[4] | FileMask[3] | FileMask[2]),
		0x0102040810204080ULL * 8 & (FileMask[7] | FileMask[6] | FileMask[5] | FileMask[4] | FileMask[3]),
		0x0102040810204080ULL * 16 & (FileMask[7] | FileMask[6] | FileMask[5] | FileMask[4]),
		0x0102040810204080ULL * 32 & (FileMask[7] | FileMask[6] | FileMask[5]),
		0x0102040810204080ULL * 64 & (FileMask[7] | FileMask[6]),
		0x0102040810204080ULL * 128 & (FileMask[7]),
	};


	MoveGen(Board* board) {
		_board = board;
	}

	//unsigned long long Perft(int depth)
	//{
	//	unsigned long long nodes = 0;

	//	if (depth == 0)
	//		return 1ULL;

	//	auto moves = GeneratePseudoLegalMoves(_board->GetTurn());
	//	std::cout << moves.size() << "\n";
	//	for (int i = 0; i < moves.size(); i++) {
	//		_board->MakeMove(&moves[i]);
	//		if (!_board->IsInCheck())
	//			nodes += Perft(depth - 1);
	//		_board->UndoMove(&moves[i]);
	//	}

	//	return nodes; 
	//}

	//int Perft(int depth) {
	//	if (!depth)
	//		return 1;

	//	int nodes = 0;

	//	for (auto move : GeneratePseudoLegalMoves(_board->GetTurn())) {
	//		_board->MakeMove(&move);
	//		nodes += Perft(depth-1);
	//		_board->UndoMove(&move);
	//	}
	//	
	//	return nodes;
	//}

	std::vector<Move> GeneratePseudoLegalMoves(Color us, int* outMoveCount = nullptr) {
		auto pieces = _board->GetColorBitboard(us);
		std::vector<Move> movelist = std::vector<Move>::vector<Move>();

		auto square = Square(-1);
		while ((square = BitHelper::PopLsbIdx(&pieces)) != 64) {
			GeneratePseudoLegalMoves(&movelist, square, us, outMoveCount);
		}

		return movelist;
	}

	std::vector<Move> GenerateLegalMoves(Color us) {
		auto pieces = _board->GetColorBitboard(us);
		std::vector<Move> movelist = std::vector<Move>::vector<Move>();

		auto square = Square(-1);
		while ((square = BitHelper::PopLsbIdx(&pieces)) != 64) {
			GenerateLegalMoves(&movelist, square, us);
		}

		return movelist;
	}

	int GeneratePseudoLegalMoves(Move* moveList, Color us) {
		auto pieces = _board->GetColorBitboard(us);

		auto square = Square(-1);
		auto pieceIdx = 0;
		while ((square = BitHelper::PopLsbIdx(&pieces)) != 64) {
			pieceIdx += GeneratePseudoLegalMoves(moveList + pieceIdx, square, us);
		}

		return pieceIdx;
	}

	Bitboard GeneratePseudoLegalMoves(const Square idx, Color us) {
		PieceType type = PieceType((_board->GetPiece(idx) & Piece::TypeMask) >> 1);
		switch (type)
		{
		default:
		case PT_NULL: return Bitboard(0);
		case Pawn: return GeneratePawnMoves(idx, us);
		case Knight: return GenerateKnightMoves(idx, us);
		case Bishop: return GenerateBishopMoves(idx, us);
		case Rook: return GenerateRookMoves(idx, us);
		case Queen: return GenerateQueenMoves(idx, us);
		case King: return GenerateKingMoves(idx, us);
		}
	}
	int GeneratePseudoLegalMoves(Move* moveList, const Square idx, Color us) {
		PieceType type = PieceType((_board->GetPiece(idx) & Piece::TypeMask) >> 1);
		switch (type)
		{
		default:
		case PT_NULL: return 0;
		case Pawn: return GeneratePawnMoves(idx, us, moveList);
		case Knight: return GenerateKnightMoves(idx, us, moveList);
		case Bishop: return GenerateBishopMoves(idx, us, moveList);
		case Rook: return GenerateRookMoves(idx, us, moveList);
		case Queen: return GenerateQueenMoves(idx, us, moveList);
		case King: return GenerateKingMoves(idx, us, moveList);
		}
	}
	std::vector<Move>* GeneratePseudoLegalMoves(std::vector<Move>* movelist, const Square idx, Color us, int* outMoveCount = nullptr) {
		PieceType type = PieceType((_board->GetPiece(idx) & Piece::TypeMask) >> 1);
		switch (type)
		{
		default:
		case PT_NULL: return movelist;
		case Pawn: return GeneratePawnMoves(idx, us, movelist);
		case Knight: return GenerateKnightMoves(idx, us, movelist);
		case Bishop: return GenerateBishopMoves(idx, us, movelist);
		case Rook: return GenerateRookMoves(idx, us, movelist);
		case Queen: return GenerateQueenMoves(idx, us, movelist);
		case King: return GenerateKingMoves(idx, us, movelist);
		}

		return movelist;
	}

	std::vector<Move>* GenerateLegalMoves(std::vector<Move>* movelist, const Square idx, Color us) {
		PieceType type = PieceType((_board->GetPiece(idx) & Piece::TypeMask) >> 1);
		switch (type)
		{
		default:
		case PT_NULL: return movelist;
		case Pawn: return GeneratePawnMoves(idx, us, movelist);
		case Knight: return GenerateKnightMoves(idx, us, movelist);
		case Bishop: return GenerateBishopMoves(idx, us, movelist);
		case Rook: return GenerateRookMoves(idx, us, movelist);
		case Queen: return GenerateQueenMoves(idx, us, movelist);
		case King: return GenerateKingMoves(idx, us, movelist);
		}

		return movelist;
	}

	Bitboard GenerateRookMoves(const Square rookIdx, Color us) {
		//Color us = Color(_board->GetPiece(rookIdx) & Piece::ColorMask); 
		Color nus = Color(!us);

		Bitboard result = 0;
		Bitboard rank = RankMask[rookIdx / 8];
		Bitboard file = FileMask[rookIdx % 8];
		Bitboard rookSquare = 1ULL << rookIdx;

		Bitboard selfDivisorDown = ~BitHelper::FromIndex(rookIdx);
		BitHelper::DeactivateBit(&selfDivisorDown, &rookIdx);

		Bitboard selfDivisorUp = BitHelper::FromIndex(rookIdx);
		BitHelper::DeactivateBit(&selfDivisorUp, &rookIdx);

		Bitboard bMask, wMask, square;

		// -< Down >-
		bMask = _board->GetColorBitboard(nus) & file & selfDivisorDown;
		wMask = (_board->GetColorBitboard(us) << 8) & (file & selfDivisorDown | rookSquare);
		square = (0xFFFFFFFFFFFFFFFF << 63 - BitHelper::MsbIdx(bMask | wMask)) & file & selfDivisorDown;
		result |= square;

		// -< Up >- 
		bMask = _board->GetColorBitboard(nus) & file & selfDivisorUp;
		wMask = (_board->GetColorBitboard(us) >> 8) & (file & selfDivisorUp | rookSquare);
		square = (0xFFFFFFFFFFFFFFFF >> 63 - BitHelper::LsbIdx(bMask | wMask)) & file & selfDivisorUp;
		result |= square;

		// -< Left >-
		bMask = _board->GetColorBitboard(nus) & rank & selfDivisorDown;
		wMask = (_board->GetColorBitboard(us) << 1) & (rank & selfDivisorDown | rookSquare);
		square = (0xFFFFFFFFFFFFFFFF << 63 - BitHelper::MsbIdx(bMask | wMask)) & rank & selfDivisorDown;
		result |= square;

		// -< Right >-
		bMask = _board->GetColorBitboard(nus) & rank & selfDivisorUp;
		wMask = (_board->GetColorBitboard(us) >> 1) & (rank & selfDivisorUp | rookSquare);
		square = (0xFFFFFFFFFFFFFFFF >> 63 - BitHelper::LsbIdx(bMask | wMask)) & rank & selfDivisorUp;
		result |= square;

		return result;
	}
	int GenerateRookMoves(const Square rookIdx, Color us, Move* movelist) {
		Color nus = Color(!us);
		int moveCount = 0;
		Bitboard moves = GenerateRookMoves(rookIdx, us);
		Square square = -1;

		while ((square = BitHelper::PopLsbIdx(&moves)) != 64) {
			MoveHelper::Flag flag = MoveHelper::QUIET_MOVE_FLAG;
			if (_board->GetColorBitboard(nus) & (1ULL << square)) { flag = MoveHelper::CAPTURE_FLAG; }
			*movelist++ = MoveHelper::Create(rookIdx, square, flag);
			moveCount++;
		}

		return moveCount;
	}
	std::vector<Move>* GenerateRookMoves(const Square rookIdx, Color us, std::vector<Move>* movelist) {
		Color nus = Color(!us);
		Bitboard moves = GenerateRookMoves(rookIdx, us);
		Square square = -1;

		while ((square = BitHelper::PopLsbIdx(&moves)) != 64) {
			MoveHelper::Flag flag = MoveHelper::QUIET_MOVE_FLAG;
			if (_board->GetColorBitboard(nus) & (1ULL << square)) { flag = MoveHelper::CAPTURE_FLAG; }
			movelist->push_back(MoveHelper::Create(rookIdx, square, flag));
		}

		return movelist;
	}

	Bitboard GenerateBishopMoves(const Square bishopIdx, Color us) {
		//Color us = Color(_board->GetPiece(bishopIdx) & Piece::ColorMask);
		Color nus = Color(!us);

		Bitboard result = 0LL;
		auto rankIdx = bishopIdx / 8;
		auto fileIdx = bishopIdx % 8;
		Bitboard self = 1ULL << bishopIdx;

		auto diag1Idx = 7 - rankIdx + fileIdx;
		Bitboard diag1 = DiagMask1[diag1Idx];

		auto diag2Idx = rankIdx + fileIdx;
		Bitboard diag2 = DiagMask2[diag2Idx];

		Bitboard bMask, wMask, square;

		Bitboard selfDivisorDown = ~BitHelper::FromIndex(bishopIdx);
		BitHelper::DeactivateBit(&selfDivisorDown, &bishopIdx);

		Bitboard selfDivisorUp = BitHelper::FromIndex(bishopIdx);
		BitHelper::DeactivateBit(&selfDivisorUp, &bishopIdx);

		// -< Bottom Left >- 
		bMask = _board->GetColorBitboard(nus) & diag1 & selfDivisorDown;
		wMask = (_board->GetColorBitboard(us) << 9) & (diag1 & selfDivisorDown | self);
		square = (0xFFFFFFFFFFFFFFFF << 63 - BitHelper::MsbIdx(bMask | wMask)) & diag1 & selfDivisorDown;
		result |= square;

		// -< Up Right >-
		bMask = _board->GetColorBitboard(nus) & diag1 & selfDivisorUp;
		wMask = (_board->GetColorBitboard(us) >> 9) & (diag1 & selfDivisorUp | self);
		square = (0xFFFFFFFFFFFFFFFF >> 63 - BitHelper::LsbIdx(bMask | wMask)) & diag1 & selfDivisorUp;
		result |= square;

		// -< Bottom Right >-
		bMask = _board->GetColorBitboard(nus) & diag2 & selfDivisorDown;
		wMask = (_board->GetColorBitboard(us) << 7) & (diag2 & selfDivisorDown | self);
		square = (0xFFFFFFFFFFFFFFFF << 63 - BitHelper::MsbIdx(bMask | wMask)) & diag2 & selfDivisorDown;
		result |= square;

		// -< Up Left >-
		bMask = _board->GetColorBitboard(nus) & diag2 & selfDivisorUp;
		wMask = (_board->GetColorBitboard(us) >> 7) & (diag2 & selfDivisorUp | self);
		square = (0xFFFFFFFFFFFFFFFF >> 63 - BitHelper::LsbIdx(bMask | wMask)) & diag2 & selfDivisorUp;
		result |= square;

		return result;
	}
	int GenerateBishopMoves(const Square bishopIdx, Color us, Move* movelist) {
		Color nus = Color(!us);
		int moveCount = 0;
		Bitboard moves = GenerateBishopMoves(bishopIdx, us);
		Square square = -1;

		while ((square = BitHelper::PopLsbIdx(&moves)) != 64) {
			MoveHelper::Flag flag = MoveHelper::QUIET_MOVE_FLAG;
			if (_board->GetColorBitboard(nus) & (1ULL << square)) { flag = MoveHelper::CAPTURE_FLAG; }
			*movelist++ = MoveHelper::Create(bishopIdx, square, flag);
			moveCount++;
		}

		return moveCount;
	}
	std::vector<Move>* GenerateBishopMoves(const Square bishopIdx, Color us, std::vector<Move>* movelist) {
		Color nus = Color(!us);
		Bitboard moves = GenerateBishopMoves(bishopIdx, us);
		Square square = -1;

		while ((square = BitHelper::PopLsbIdx(&moves)) != 64) {
			MoveHelper::Flag flag = MoveHelper::QUIET_MOVE_FLAG;
			if (_board->GetColorBitboard(nus) & (1ULL << square)) { flag = MoveHelper::CAPTURE_FLAG; }
			movelist->push_back(MoveHelper::Create(bishopIdx, square, flag));
		}

		return movelist;
	}

	Bitboard GenerateKingMoves(const Square kingIdx, Color us) {
		//Color us = Color(_board->GetPiece(kingIdx) & Piece::ColorMask);
		Color nus = Color(!us);
		Bitboard result = 0ULL;
		auto rankIdx = kingIdx / 8;
		auto fileIdx = kingIdx % 8;
		Bitboard allies = _board->GetColorBitboard(us);
		Bitboard pieces = allies | _board->GetColorBitboard(nus);

		auto ranks = RankMask[rankIdx];
		if (rankIdx < 7) ranks |= RankMask[rankIdx + 1];
		if (rankIdx > 0) ranks |= RankMask[rankIdx - 1];

		auto files = FileMask[fileIdx];
		if (fileIdx < 7) files |= FileMask[fileIdx + 1];
		if (fileIdx > 0) files |= FileMask[fileIdx - 1];

		result = files & ranks;

		// Exclude allies and the king himself
		result ^= allies & files & ranks;

		// King side castling
		if (_board->GetCastlingRights(us, true) && !(pieces & 0x60ULL << us * 56)) {
			result |= RankMask[rankIdx] & FileMask[FileTable::H];
		}

		// Queen side castling
		if (_board->GetCastlingRights(us, false) && !(pieces & 0xEULL << us * 56)) {
			result |= RankMask[rankIdx] & FileMask[FileTable::A];
		}

		return result;
	}
	int GenerateKingMoves(const Square kingIdx, Color us, Move* movelist) {
		Color nus = Color(!us);
		int moveCount = 0;
		Bitboard moves = GenerateKingMoves(kingIdx, us);
		Square square = -1;

		while ((square = BitHelper::PopLsbIdx(&moves)) != 64) {
			MoveHelper::Flag flag = MoveHelper::QUIET_MOVE_FLAG;
			if (_board->GetColorBitboard(nus) & (1ULL << square)) { flag = MoveHelper::CAPTURE_FLAG; }
			if (_board->GetCastlingRights(us, true) && square % 8 == FileTable::H) { flag = MoveHelper::KING_CASTLE_FLAG; }
			if (_board->GetCastlingRights(us, true) && square % 8 == FileTable::A) { flag = MoveHelper::QUEEN_CASTLE_FLAG; }
			*movelist++ = MoveHelper::Create(kingIdx, square, flag);
			moveCount++;
		}

		return moveCount;
	}
	std::vector<Move>* GenerateKingMoves(const Square kingIdx, Color us, std::vector<Move>* movelist) {
		Color nus = Color(!us);
		Bitboard moves = GenerateKingMoves(kingIdx, us);
		Square square = -1;

		while ((square = BitHelper::PopLsbIdx(&moves)) != 64) {
			MoveHelper::Flag flag = MoveHelper::QUIET_MOVE_FLAG;
			if (_board->GetColorBitboard(nus) & (1ULL << square)) { flag = MoveHelper::CAPTURE_FLAG; }
			movelist->push_back(MoveHelper::Create(kingIdx, square, flag));
		}

		return movelist;
	}

	Bitboard GenerateQueenMoves(const Square queenIdx, Color us) {
		//Color us = Color(_board->GetPiece(queenIdx) & Piece::ColorMask);
		Color nus = Color(!us);

		return GenerateRookMoves(queenIdx, us) | GenerateBishopMoves(queenIdx, us);
	}
	int GenerateQueenMoves(const Square queenIdx, Color us, Move* movelist) {
		Color nus = Color(!us);
		int moveCount = 0;
		Bitboard moves = GenerateQueenMoves(queenIdx, us);
		Square square = -1;

		while ((square = BitHelper::PopLsbIdx(&moves)) != 64) {
			MoveHelper::Flag flag = MoveHelper::QUIET_MOVE_FLAG;
			if (_board->GetColorBitboard(nus) & (1ULL << square)) { flag = MoveHelper::CAPTURE_FLAG; }
			*movelist++ = MoveHelper::Create(queenIdx, square, flag);
			moveCount++;
		}

		return moveCount;
	}
	std::vector<Move>* GenerateQueenMoves(const Square queenIdx, Color us, std::vector<Move>* movelist) {
		Color nus = Color(!us);
		Bitboard moves = GenerateQueenMoves(queenIdx, us);
		Square square = -1;

		while ((square = BitHelper::PopLsbIdx(&moves)) != 64) {
			MoveHelper::Flag flag = MoveHelper::QUIET_MOVE_FLAG;
			if (_board->GetColorBitboard(nus) & (1ULL << square)) { flag = MoveHelper::CAPTURE_FLAG; }
			movelist->push_back(MoveHelper::Create(queenIdx, square, flag));
		}

		return movelist;
	}

	Bitboard GenerateKnightMoves(const Square knightIdx, Color us) {
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
	int GenerateKnightMoves(const Square knightIdx, Color us, Move* movelist) {
		Color nus = Color(!us);
		int moveCount = 0;
		Bitboard moves = GenerateKnightMoves(knightIdx, us);
		Square square = -1;

		while ((square = BitHelper::PopLsbIdx(&moves)) != 64) {
			MoveHelper::Flag flag = MoveHelper::QUIET_MOVE_FLAG;
			if (_board->GetColorBitboard(nus) & (1ULL << square)) { flag = MoveHelper::CAPTURE_FLAG; }
			*movelist++ = MoveHelper::Create(knightIdx, square, flag);
			moveCount++;
		}

		return moveCount;
	}
	std::vector<Move>* GenerateKnightMoves(const Square knightIdx, Color us, std::vector<Move>* movelist) {
		Color nus = Color(!us);
		Bitboard moves = GenerateKnightMoves(knightIdx, us);
		Square square = -1;

		while ((square = BitHelper::PopLsbIdx(&moves)) != 64) {
			MoveHelper::Flag flag = MoveHelper::QUIET_MOVE_FLAG;
			if (_board->GetColorBitboard(nus) & (1ULL << square)) { flag = MoveHelper::CAPTURE_FLAG; }
			movelist->push_back(MoveHelper::Create(knightIdx, square, flag));
		}

		return movelist;
	}

	Bitboard GeneratePawnMoves(const Square pawnIdx, Color us) {
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
		/*else {
			std::cout << (enemies | friends) << "\n";
			std::cout << ((enemies | friends) & forward1) << "\n";
		}*/

		// Capture left 
		Bitboard captureLeft = 1ULL << pawnIdx + (us == Color::White ? 7 : -9);
		if (fileIdx > 0 && enemies & captureLeft) {
			result |= captureLeft;
		}

		// Capture right
		Bitboard captureRight = 1ULL << pawnIdx + (us == Color::White ? 9 : -7);
		if (fileIdx < 7 && enemies & captureRight) {
			result |= captureRight;
		}

		// En passant
		Bitboard enPassant = _board->GetEnPassantBitboard();
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
	int GeneratePawnMoves(const Square pawnIdx, Color us, Move* movelist) {
		Color nus = Color(!us);
		int moveCount = 0;
		Bitboard moves = GeneratePawnMoves(pawnIdx, us);
		Square square = -1;

		while ((square = BitHelper::PopLsbIdx(&moves)) != 64) {
			if (square / 8 == 0 || square / 8 == 7) {
				*movelist++ = MoveHelper::Create(pawnIdx, square, MoveHelper::PROMOTION_BISHOP_FLAG);
				*movelist++ = MoveHelper::Create(pawnIdx, square, MoveHelper::PROMOTION_KNIGHT_FLAG);
				*movelist++ = MoveHelper::Create(pawnIdx, square, MoveHelper::PROMOTION_QUEEN_FLAG);
				*movelist++ = MoveHelper::Create(pawnIdx, square, MoveHelper::PROMOTION_ROOK_FLAG);
				moveCount += 4;
				if (_board->GetColorBitboard(nus) & (1ULL << square)) {
					*movelist++ = MoveHelper::Create(pawnIdx, square, MoveHelper::CAPTURE_PROMOTION_BISHOP_FLAG);
					*movelist++ = MoveHelper::Create(pawnIdx, square, MoveHelper::CAPTURE_PROMOTION_KNIGHT_FLAG);
					*movelist++ = MoveHelper::Create(pawnIdx, square, MoveHelper::CAPTURE_PROMOTION_QUEEN_FLAG);
					*movelist++ = MoveHelper::Create(pawnIdx, square, MoveHelper::CAPTURE_PROMOTION_ROOK_FLAG);
					moveCount += 4;
				}
			}
			else {
				MoveHelper::Flag flag = MoveHelper::QUIET_MOVE_FLAG;
				if (_board->GetColorBitboard(nus) & (1ULL << square)) { flag = MoveHelper::CAPTURE_FLAG; }
				if (_board->GetEnPassantBitboard() & (1ULL << square))
					flag = MoveHelper::EN_PASSANT_FLAG;
				if (pawnIdx - square == 16 || pawnIdx - square == -16) { flag = MoveHelper::DOUBLE_PAWN_PUSH_FLAG; }

				*movelist++ = MoveHelper::Create(pawnIdx, square, flag);
				moveCount++;
			}
		}

		return moveCount;
	}
	/*std::vector<Move>* GeneratePawnMoves(const Square pawnIdx, Color us, std::vector<Move>* movelist, int* outMoveCount = nullptr) {
		Color nus = Color(!us);
		Bitboard moves = GeneratePawnMoves(pawnIdx, us);
		Square square = -1;
		int moveCount = 0;

		while ((square = BitHelper::PopLsbIdx(&moves)) != 64) {
			++moveCount;
			if (square / 8 == 0 || square / 8 == 7) {
				movelist->push_back(MoveHelper::Create(pawnIdx, square, MoveHelper::PROMOTION_BISHOP_FLAG));
				movelist->push_back(MoveHelper::Create(pawnIdx, square, MoveHelper::PROMOTION_KNIGHT_FLAG));
				movelist->push_back(MoveHelper::Create(pawnIdx, square, MoveHelper::PROMOTION_QUEEN_FLAG));
				movelist->push_back(MoveHelper::Create(pawnIdx, square, MoveHelper::PROMOTION_ROOK_FLAG));
				if (_board->GetColorBitboard(nus) & (1ULL << square)) {
					movelist->push_back(MoveHelper::Create(pawnIdx, square, MoveHelper::CAPTURE_PROMOTION_BISHOP_FLAG));
					movelist->push_back(MoveHelper::Create(pawnIdx, square, MoveHelper::CAPTURE_PROMOTION_KNIGHT_FLAG));
					movelist->push_back(MoveHelper::Create(pawnIdx, square, MoveHelper::CAPTURE_PROMOTION_QUEEN_FLAG));
					movelist->push_back(MoveHelper::Create(pawnIdx, square, MoveHelper::CAPTURE_PROMOTION_ROOK_FLAG));
				}
			}
			else {
				MoveHelper::Flag flag = MoveHelper::QUIET_MOVE_FLAG;
				if (_board->GetColorBitboard(nus) & (1ULL << square)) { flag = MoveHelper::CAPTURE_FLAG; }
				if (_board->GetEnPassantBitboard() & (1ULL << square)) { flag = MoveHelper::EN_PASSANT_FLAG; }
				if (pawnIdx - square == 16 || pawnIdx - square == -16) { flag = MoveHelper::DOUBLE_PAWN_PUSH_FLAG; }

				movelist->push_back(MoveHelper::Create(pawnIdx, square, flag));
			}
		}

		return movelist;
	}*/
	std::vector<Move>* GeneratePawnMoves(const Square pawnIdx, Color us, std::vector<Move>* movelist) {
		Color nus = Color(!us);

		auto rankIdx = pawnIdx / 8;
		auto fileIdx = pawnIdx % 8;

		Bitboard enemies = _board->GetColorBitboard(nus);
		Bitboard friends = _board->GetColorBitboard(us);

		const Square captureLeftIdx = pawnIdx + (us == Color::White ? 7 : -9);
		const Square captureRightIdx = pawnIdx + (us == Color::White ? 9 : -7);

		// Move 1 square forward
		// We don't need to check wether the pawn is on the last rank or not, which might cause overflow,
		// as the pawn can never be on the last rank. If it is moved there, it will have to promote.
		Square forward1Idx = pawnIdx + (us == Color::White ? 8 : -8);
		Bitboard forward1 = 1ULL << forward1Idx; // By definition this has always only one set bit
		if (!((enemies | friends) & forward1)) {
			//result |= forward1;
			if (rankIdx == 7 || rankIdx == 0) {
				// promo
				movelist->push_back(MoveHelper::Create(pawnIdx, forward1Idx, MoveHelper::PROMOTION_BISHOP_FLAG));
				movelist->push_back(MoveHelper::Create(pawnIdx, forward1Idx, MoveHelper::PROMOTION_KNIGHT_FLAG));
				movelist->push_back(MoveHelper::Create(pawnIdx, forward1Idx, MoveHelper::PROMOTION_QUEEN_FLAG));
				movelist->push_back(MoveHelper::Create(pawnIdx, forward1Idx, MoveHelper::PROMOTION_ROOK_FLAG));
			}
			else {
				// normal move
				movelist->push_back(MoveHelper::Create(pawnIdx, forward1Idx, MoveHelper::QUIET_MOVE_FLAG));
			}

			// Move 2 squares forward
			// If we can't move forward one square,
			// we won't be able to move forward two squares.
			//	=> This is nested inside the `forward1` condition 
			Square forward2Idx = pawnIdx + (us == Color::White ? 16 : -16);
			Bitboard forward2 = 1ULL << forward2Idx; // By definition this has always only one set bit
			if (rankIdx == (us == Color::White ? 1 : 6) && !((enemies | friends) & forward2)) {
				//result |= forward2;
				movelist->push_back(MoveHelper::Create(pawnIdx, forward2Idx, MoveHelper::DOUBLE_PAWN_PUSH_FLAG));
			}
		}

		// Capture left 
		Bitboard captureLeft = 1ULL << captureLeftIdx;
		if (fileIdx > 0 && enemies & captureLeft) {
			if (rankIdx == 7 || rankIdx == 0) {
				// promo capture
				movelist->push_back(MoveHelper::Create(pawnIdx, captureLeftIdx, MoveHelper::CAPTURE_PROMOTION_BISHOP_FLAG));
				movelist->push_back(MoveHelper::Create(pawnIdx, captureLeftIdx, MoveHelper::CAPTURE_PROMOTION_KNIGHT_FLAG));
				movelist->push_back(MoveHelper::Create(pawnIdx, captureLeftIdx, MoveHelper::CAPTURE_PROMOTION_QUEEN_FLAG));
				movelist->push_back(MoveHelper::Create(pawnIdx, captureLeftIdx, MoveHelper::CAPTURE_PROMOTION_ROOK_FLAG));
			}
			else {
				// normal capture
				movelist->push_back(MoveHelper::Create(pawnIdx, captureLeftIdx, MoveHelper::CAPTURE_FLAG));
			}
		}

		// Capture right
		Bitboard captureRight = 1ULL << captureRightIdx;
		if (fileIdx < 7 && enemies & captureRight) {
			if (rankIdx == 7 || rankIdx == 0) {
				// promo capture
				movelist->push_back(MoveHelper::Create(pawnIdx, captureRightIdx, MoveHelper::CAPTURE_PROMOTION_BISHOP_FLAG));
				movelist->push_back(MoveHelper::Create(pawnIdx, captureRightIdx, MoveHelper::CAPTURE_PROMOTION_KNIGHT_FLAG));
				movelist->push_back(MoveHelper::Create(pawnIdx, captureRightIdx, MoveHelper::CAPTURE_PROMOTION_QUEEN_FLAG));
				movelist->push_back(MoveHelper::Create(pawnIdx, captureRightIdx, MoveHelper::CAPTURE_PROMOTION_ROOK_FLAG));
			}
			else {
				// normal capture
				movelist->push_back(MoveHelper::Create(pawnIdx, captureRightIdx, MoveHelper::CAPTURE_FLAG));
			}
		}

		// En passant
		assert(captureRightIdx != -1);
		assert(captureLeftIdx != -1);
		if ((fileIdx < 7 && (_board->GetEnPassantSquare() == captureRightIdx)) ||	// right
			(fileIdx > 0 && (_board->GetEnPassantSquare() == captureLeftIdx))) {	// left
			movelist->push_back(MoveHelper::Create(pawnIdx, _board->GetEnPassantSquare(), MoveHelper::EN_PASSANT_FLAG));
		}

		return movelist;
	}
	std::vector<Move>* GenerateLegalPawnMoves(const Square pawnIdx, Color us, std::vector<Move>* movelist) {
		Color nus = Color(!us);
		auto rankIdx = pawnIdx / 8;
		auto fileIdx = pawnIdx % 8;
		Bitboard enemies = _board->GetColorBitboard(nus);
		Bitboard friends = _board->GetColorBitboard(us);
		const Square captureLeftIdx = pawnIdx + (us == Color::White ? 7 : -9);
		const Square captureRightIdx = pawnIdx + (us == Color::White ? 9 : -7);

		// Move 1 square forward
		// We don't need to check wether the pawn is on the last rank or not, which might cause overflow,
		// as the pawn can never be on the last rank. If it is moved there, it will have to promote.
		if (_board->IsInCheck()) {

		}
		Square forward1Idx = pawnIdx + (us == Color::White ? 8 : -8);
		Bitboard forward1 = 1ULL << forward1Idx; // By definition this has always only one set bit
		if (!((enemies | friends) & forward1)) {
			//result |= forward1;
			if (rankIdx == 7 || rankIdx == 0) {
				// promo
				movelist->push_back(MoveHelper::Create(pawnIdx, forward1Idx, MoveHelper::PROMOTION_BISHOP_FLAG));
				movelist->push_back(MoveHelper::Create(pawnIdx, forward1Idx, MoveHelper::PROMOTION_KNIGHT_FLAG));
				movelist->push_back(MoveHelper::Create(pawnIdx, forward1Idx, MoveHelper::PROMOTION_QUEEN_FLAG));
				movelist->push_back(MoveHelper::Create(pawnIdx, forward1Idx, MoveHelper::PROMOTION_ROOK_FLAG));
			}
			else {
				// normal move
				movelist->push_back(MoveHelper::Create(pawnIdx, forward1Idx, MoveHelper::QUIET_MOVE_FLAG));
			}

			// Move 2 squares forward
			// If we can't move forward one square,
			// we won't be able to move forward two squares.
			//	=> This is nested inside the `forward1` condition 
			Square forward2Idx = pawnIdx + (us == Color::White ? 16 : -16);
			Bitboard forward2 = 1ULL << forward2Idx; // By definition this has always only one set bit
			if (rankIdx == (us == Color::White ? 1 : 6) && !((enemies | friends) & forward2)) {
				//result |= forward2;
				movelist->push_back(MoveHelper::Create(pawnIdx, forward2Idx, MoveHelper::DOUBLE_PAWN_PUSH_FLAG));
			}
		}

		// Capture left 
		Bitboard captureLeft = 1ULL << captureLeftIdx;
		if (fileIdx > 0 && enemies & captureLeft) {
			if (rankIdx == 7 || rankIdx == 0) {
				// promo capture
				movelist->push_back(MoveHelper::Create(pawnIdx, captureLeftIdx, MoveHelper::CAPTURE_PROMOTION_BISHOP_FLAG));
				movelist->push_back(MoveHelper::Create(pawnIdx, captureLeftIdx, MoveHelper::CAPTURE_PROMOTION_KNIGHT_FLAG));
				movelist->push_back(MoveHelper::Create(pawnIdx, captureLeftIdx, MoveHelper::CAPTURE_PROMOTION_QUEEN_FLAG));
				movelist->push_back(MoveHelper::Create(pawnIdx, captureLeftIdx, MoveHelper::CAPTURE_PROMOTION_ROOK_FLAG));
			}
			else {
				// normal capture
				movelist->push_back(MoveHelper::Create(pawnIdx, captureLeftIdx, MoveHelper::CAPTURE_FLAG));
			}
		}

		// Capture right
		Bitboard captureRight = 1ULL << captureRightIdx;
		if (fileIdx < 7 && enemies & captureRight) {
			if (rankIdx == 7 || rankIdx == 0) {
				// promo capture
				movelist->push_back(MoveHelper::Create(pawnIdx, captureRightIdx, MoveHelper::CAPTURE_PROMOTION_BISHOP_FLAG));
				movelist->push_back(MoveHelper::Create(pawnIdx, captureRightIdx, MoveHelper::CAPTURE_PROMOTION_KNIGHT_FLAG));
				movelist->push_back(MoveHelper::Create(pawnIdx, captureRightIdx, MoveHelper::CAPTURE_PROMOTION_QUEEN_FLAG));
				movelist->push_back(MoveHelper::Create(pawnIdx, captureRightIdx, MoveHelper::CAPTURE_PROMOTION_ROOK_FLAG));
			}
			else {
				// normal capture
				movelist->push_back(MoveHelper::Create(pawnIdx, captureRightIdx, MoveHelper::CAPTURE_FLAG));
			}
		}

		// En passant
		assert(captureRightIdx != -1);
		assert(captureLeftIdx != -1);
		if ((fileIdx < 7 && (_board->GetEnPassantSquare() == captureRightIdx)) ||	// right
			(fileIdx > 0 && (_board->GetEnPassantSquare() == captureLeftIdx))) {	// left
			movelist->push_back(MoveHelper::Create(pawnIdx, _board->GetEnPassantSquare(), MoveHelper::EN_PASSANT_FLAG));
		}
		return movelist;
	}
};