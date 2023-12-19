#include "pch.h"

#include <iostream>
#include <algorithm>

#include "MoveGen.h"
#include "BitHelper.h"
#include "Misc.h"
#include "MoveHelper.h"


namespace Machiavelli {

	MoveGen::MoveGen(Board* b)
	{
		_board = b;
	}


	int MoveGen::Perft(int depth, bool pv)
	{
		if (depth == 0)
			return 1;

		unsigned long long moveCount = 0;
		auto movelist = GenerateLegalMoves();
		for (int i = 0; i < movelist.size(); i++) {
			_board->MakeMove(&movelist[i]);
			int count = Perft(depth - 1, false);
			if (pv) std::cout << MoveHelper::ToString(movelist[i]) << ": " << count << "\n";
			moveCount += count;
			_board->UndoMove(&movelist[i]);
		}

		if (pv) {
			std::cout << "Nodes searched: " << moveCount << '\n';
		}

		return moveCount;
	}

	std::vector<Move> MoveGen::GeneratePseudoLegalMoves()
	{
		return GeneratePseudoLegalMoves(_board->GetTurn());
	}

	std::vector<Move> MoveGen::GeneratePseudoLegalMoves(Color c)
	{
		auto pieces = _board->GetColorBitboard(c);
		std::vector<Move> movelist = std::vector<Move>::vector();

		auto square = Square(-1);
		while ((square = BitHelper::PopLsbIdx(&pieces)) != 64) {
			switch (PieceType((_board->GetPiece(square) & Piece::TypeMask) >> 1))
			{
			default: throw std::invalid_argument("Invalid piece type");
			case PT_NULL: break;
			case Pawn: GeneratePseudoLegalPawnMoves(square, c, &movelist); break;
			case Knight: GeneratePseudoLegalKnightMoves(square, c, &movelist); break;
			case Bishop: GeneratePseudoLegalBishopMoves(square, c, &movelist); break;
			case Rook: GeneratePseudoLegalRookMoves(square, c, &movelist); break;
			case Queen: GeneratePseudoLegalQueenMoves(square, c, &movelist); break;
			case King: GeneratePseudoLegalKingMoves(square, c, &movelist); break;
			}
		}

		return movelist;
	}

	Bitboard MoveGen::GenerateAttacks(const Square idx, Color us)
	{
		switch (PieceType((_board->GetPiece(idx) & Piece::TypeMask) >> 1))
		{
		default: throw std::invalid_argument("Invalid piece type.");
		case PT_NULL: return Bitboard(0);
		case Pawn: return GeneratePawnAttacks(idx, us);
		case Knight: return GenerateKnightAttacks(idx, us);
		case Bishop: return GenerateBishopAttacks(idx, us);
		case Rook: return GenerateRookAttacks(idx, us);
		case Queen: return GenerateQueenAttacks(idx, us);
		case King:return  GenerateKingAttacks(idx, us);
		}
	}

	void MoveGen::GeneratePseudoLegalPawnMoves(const Square idx, Color us, std::vector<Move>* movelist) {
		Color nus = Color(!us);

		auto rankIdx = idx / 8;
		auto fileIdx = idx % 8;

		Bitboard enemies = _board->GetColorBitboard(nus);
		Bitboard friends = _board->GetColorBitboard(us);

		const Square captureLeftIdx = idx + (us == Color::White ? 7 : -9);
		const Square captureRightIdx = idx + (us == Color::White ? 9 : -7);

		// Move 1 square forward
		// We don't need to check wether the pawn is on the last rank or not, which might cause overflow,
		// as the pawn can never be on the last rank. If it is moved there, it will have to promote.
		Square forward1Idx = idx + (us == Color::White ? 8 : -8);
		Bitboard forward1 = 1ULL << forward1Idx; // By definition this has always only one set bit
		if (!((enemies | friends) & forward1)) {
			//result |= forward1;
			if (rankIdx == 7 || rankIdx == 0) {
				// promo
				movelist->push_back(MoveHelper::Create(idx, forward1Idx, MoveHelper::PROMOTION_BISHOP_FLAG));
				movelist->push_back(MoveHelper::Create(idx, forward1Idx, MoveHelper::PROMOTION_KNIGHT_FLAG));
				movelist->push_back(MoveHelper::Create(idx, forward1Idx, MoveHelper::PROMOTION_QUEEN_FLAG));
				movelist->push_back(MoveHelper::Create(idx, forward1Idx, MoveHelper::PROMOTION_ROOK_FLAG));
			}
			else {
				// normal move
				movelist->push_back(MoveHelper::Create(idx, forward1Idx, MoveHelper::QUIET_MOVE_FLAG));
			}

			// Move 2 squares forward
			// If we can't move forward one square,
			// we won't be able to move forward two squares.
			//	=> This is nested inside the `forward1` condition 
			Square forward2Idx = idx + (us == Color::White ? 16 : -16);
			Bitboard forward2 = 1ULL << forward2Idx; // By definition this has always only one set bit
			if (rankIdx == (us == Color::White ? 1 : 6) && !((enemies | friends) & forward2)) {
				//result |= forward2;
				movelist->push_back(MoveHelper::Create(idx, forward2Idx, MoveHelper::DOUBLE_PAWN_PUSH_FLAG));
			}
		}

		// Capture left 
		Bitboard captureLeft = 1ULL << captureLeftIdx;
		if (fileIdx > 0 && enemies & captureLeft) {
			if (rankIdx == 7 || rankIdx == 0) {
				// promo capture
				movelist->push_back(MoveHelper::Create(idx, captureLeftIdx, MoveHelper::CAPTURE_PROMOTION_BISHOP_FLAG));
				movelist->push_back(MoveHelper::Create(idx, captureLeftIdx, MoveHelper::CAPTURE_PROMOTION_KNIGHT_FLAG));
				movelist->push_back(MoveHelper::Create(idx, captureLeftIdx, MoveHelper::CAPTURE_PROMOTION_QUEEN_FLAG));
				movelist->push_back(MoveHelper::Create(idx, captureLeftIdx, MoveHelper::CAPTURE_PROMOTION_ROOK_FLAG));
			}
			else {
				// normal capture
				movelist->push_back(MoveHelper::Create(idx, captureLeftIdx, MoveHelper::CAPTURE_FLAG));
			}
		}

		// Capture right
		Bitboard captureRight = 1ULL << captureRightIdx;
		if (fileIdx < 7 && enemies & captureRight) {
			if (rankIdx == 7 || rankIdx == 0) {
				// promo capture
				movelist->push_back(MoveHelper::Create(idx, captureRightIdx, MoveHelper::CAPTURE_PROMOTION_BISHOP_FLAG));
				movelist->push_back(MoveHelper::Create(idx, captureRightIdx, MoveHelper::CAPTURE_PROMOTION_KNIGHT_FLAG));
				movelist->push_back(MoveHelper::Create(idx, captureRightIdx, MoveHelper::CAPTURE_PROMOTION_QUEEN_FLAG));
				movelist->push_back(MoveHelper::Create(idx, captureRightIdx, MoveHelper::CAPTURE_PROMOTION_ROOK_FLAG));
			}
			else {
				// normal capture
				movelist->push_back(MoveHelper::Create(idx, captureRightIdx, MoveHelper::CAPTURE_FLAG));
			}
		}

		// En passant
		auto enpassantSqr = _board->GetEnPassantSquare();
		if ((fileIdx < 7 && (enpassantSqr == captureRightIdx)) ||	// right
			(fileIdx > 0 && (enpassantSqr == captureLeftIdx))		// left
			&& enpassantSqr >= 0) {
			movelist->push_back(MoveHelper::Create(idx, _board->GetEnPassantSquare(), MoveHelper::EN_PASSANT_FLAG));
		}
	}

	Bitboard MoveGen::GeneratePawnAttacks(const Square idx, Color us)
	{
		Bitboard result = 0ULL;
		const Square fileIdx = idx % 8;
		const Square captureLeftIdx = idx + (us == Color::White ? 7 : -9);
		const Square captureRightIdx = idx + (us == Color::White ? 9 : -7);

		// Capture left 
		if (fileIdx > 0) {
			result |= 1ULL << captureLeftIdx;
		}

		// Capture right
		if (fileIdx < 7) {
			result |= 1ULL << captureRightIdx;
		}

		// En passant
		auto enpassantSqr = _board->GetEnPassantSquare();
		if ((fileIdx < 7 && (enpassantSqr == captureRightIdx)) ||	// right
			(fileIdx > 0 && (enpassantSqr == captureLeftIdx))		// left
			&& enpassantSqr >= 0) { // en passant possible?
			result |= 1ULL << enpassantSqr;
		}

		return result;
	}

	void MoveGen::GeneratePseudoLegalKnightMoves(const Square idx, Color us, std::vector<Move>* movelist)
	{
		auto rankIdx = idx / 8;
		auto fileIdx = idx % 8;
		auto enemies = _board->GetColorBitboard(Color(!us));
		auto friends = _board->GetColorBitboard(us);

		auto addMove = [&](int nidx) {
			Bitboard sqmk = 1ULL << nidx;
			if (!(friends & sqmk)) {
				movelist->push_back(MoveHelper::Create(idx, nidx, enemies & sqmk ? MoveHelper::CAPTURE_FLAG : MoveHelper::QUIET_MOVE_FLAG));
			}
		};

		// Include typical squares
		// upper
		if (rankIdx < 6) {
			// right 
			if (fileIdx < 7) addMove(idx + 17);
			// left
			if (fileIdx > 0) addMove(idx + 15);
		}
		// up
		if (rankIdx < 7) {
			// righter
			if (fileIdx < 6) addMove(idx + 10);
			// lefter
			if (fileIdx > 1) addMove(idx + 6);
		}
		// lower
		if (rankIdx > 1) {
			// left
			if (fileIdx > 0) addMove(idx - 17);
			// right
			if (fileIdx < 7) addMove(idx - 15);
		}
		// low
		if (rankIdx > 0) {
			// lefter
			if (fileIdx > 1) addMove(idx - 10);
			// righter
			if (fileIdx < 6) addMove(idx - 6);
		}
	}

	Bitboard MoveGen::GenerateKnightAttacks(const Square idx, Color us)
	{
		const auto rankIdx = idx / 8;
		const auto fileIdx = idx % 8;
		Bitboard result = Bitboard();

		// Include typical squares
		// upper
		if (rankIdx < 6) {
			// right 
			if (fileIdx < 7) result |= 1ULL << 17 + idx;
			// left
			if (fileIdx > 0) result |= 1ULL << 15 + idx;
		}
		// up
		if (rankIdx < 7) {
			// righter
			if (fileIdx < 6) result |= 1ULL << 10 + idx;
			// lefter
			if (fileIdx > 1) result |= 1ULL << 6 + idx;
		}
		// lower
		if (rankIdx > 1) {
			// left
			if (fileIdx > 0) result |= 1ULL << idx - 17;
			// right
			if (fileIdx < 7) result |= 1ULL << idx - 15;
		}
		// low
		if (rankIdx > 0) {
			// lefter
			if (fileIdx > 1) result |= 1ULL << idx - 10;
			// righter
			if (fileIdx < 6) result |= 1ULL << idx - 6;
		}

		return result;
	}

	void MoveGen::GeneratePseudoLegalBishopMoves(const Square idx, Color us, std::vector<Move>* movelist)
	{
		Color nus = Color(!us);
		Bitboard moves = GeneratePseudoLegalBishopAttacks(idx, us);
		Square square = -1;

		while ((square = BitHelper::PopLsbIdx(&moves)) != 64) {
			MoveHelper::Flag flag = MoveHelper::QUIET_MOVE_FLAG;
			if (_board->GetColorBitboard(nus) & (1ULL << square)) { flag = MoveHelper::CAPTURE_FLAG; }
			movelist->push_back(MoveHelper::Create(idx, square, flag));
		}
	}
	Bitboard MoveGen::GeneratePseudoLegalBishopAttacks(const Square idx, Color us)
	{
		Color nus = Color(!us);

		Bitboard result = 0LL;
		auto rankIdx = idx / 8;
		auto fileIdx = idx % 8;
		Bitboard self = 1ULL << idx;

		auto diag1Idx = 7 - rankIdx + fileIdx;
		Bitboard diag1 = DiagMask1[diag1Idx];

		auto diag2Idx = rankIdx + fileIdx;
		Bitboard diag2 = DiagMask2[diag2Idx];

		Bitboard bMask, wMask, square;

		Bitboard selfDivisorDown = ~BitHelper::FromIndex(idx);
		BitHelper::DeactivateBit(&selfDivisorDown, &idx);

		Bitboard selfDivisorUp = BitHelper::FromIndex(idx);
		BitHelper::DeactivateBit(&selfDivisorUp, &idx);

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
	Bitboard MoveGen::GenerateBishopAttacks(const Square idx, Color us)
	{
		Color nus = Color(!us);

		Bitboard result = 0LL;
		auto rankIdx = idx / 8;
		auto fileIdx = idx % 8;
		Bitboard self = 1ULL << idx;

		auto diag1Idx = 7 - rankIdx + fileIdx;
		Bitboard diag1 = DiagMask1[diag1Idx];

		auto diag2Idx = rankIdx + fileIdx;
		Bitboard diag2 = DiagMask2[diag2Idx];

		Bitboard selfDivisorDown = ~BitHelper::FromIndex(idx);
		BitHelper::DeactivateBit(&selfDivisorDown, &idx);

		Bitboard selfDivisorUp = BitHelper::FromIndex(idx);
		BitHelper::DeactivateBit(&selfDivisorUp, &idx);

		const Bitboard pieces = (_board->GetColorBitboard(us) | _board->GetColorBitboard(nus)) ^ self;
		const Bitboard diag1Pieces = pieces & diag1;
		const Bitboard diag2Pieces = pieces & diag2;
		Bitboard squares;

		// -< Bottom Left >- 
		squares = 0xFFFFFFFFFFFFFFFF << 63 - BitHelper::MsbIdx(diag1Pieces & selfDivisorDown);
		result |= squares & diag1 & selfDivisorDown;

		//// -< Up Right >-
		squares = 0xFFFFFFFFFFFFFFFF >> 63 - BitHelper::LsbIdx(diag1Pieces & selfDivisorUp);
		result |= squares & diag1 & selfDivisorUp;

		//// -< Bottom Right >-
		squares = 0xFFFFFFFFFFFFFFFF << 63 - BitHelper::MsbIdx(diag2Pieces & selfDivisorDown);
		result |= squares & diag2 & selfDivisorDown;

		//// -< Up Left >-
		squares = 0xFFFFFFFFFFFFFFFF >> 63 - BitHelper::LsbIdx(diag2Pieces & selfDivisorUp);
		result |= squares & diag2 & selfDivisorUp;

		return result;
	}

	void MoveGen::GeneratePseudoLegalRookMoves(const Square idx, Color us, std::vector<Move>* movelist)
	{
		Color nus = Color(!us);
		Bitboard moves = GeneratePseudoLegalRookAttacks(idx, us);
		Square square = -1;

		while ((square = BitHelper::PopLsbIdx(&moves)) != 64) {
			MoveHelper::Flag flag = MoveHelper::QUIET_MOVE_FLAG;
			if (_board->GetColorBitboard(nus) & (1ULL << square)) { flag = MoveHelper::CAPTURE_FLAG; }
			movelist->push_back(MoveHelper::Create(idx, square, flag));
		}
	}
	Bitboard MoveGen::GeneratePseudoLegalRookAttacks(const Square idx, Color us)
	{
		Color nus = Color(!us);

		Bitboard result = 0;
		Bitboard rank = RankMask[idx / 8];
		Bitboard file = FileMask[idx % 8];
		Bitboard rookSquare = 1ULL << idx;

		Bitboard selfDivisorDown = ~BitHelper::FromIndex(idx);
		BitHelper::DeactivateBit(&selfDivisorDown, &idx);

		Bitboard selfDivisorUp = BitHelper::FromIndex(idx);
		BitHelper::DeactivateBit(&selfDivisorUp, &idx);

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
	Bitboard MoveGen::GenerateRookAttacks(const Square idx, Color us)
	{
		Color nus = Color(!us);

		Bitboard result = 0;
		Bitboard rank = RankMask[idx / 8];
		Bitboard file = FileMask[idx % 8];
		Bitboard rookSquare = 1ULL << idx;

		Bitboard selfDivisorDown = ~BitHelper::FromIndex(idx);
		BitHelper::DeactivateBit(&selfDivisorDown, &idx);

		Bitboard selfDivisorUp = BitHelper::FromIndex(idx);
		BitHelper::DeactivateBit(&selfDivisorUp, &idx);

		const Bitboard pieces = (_board->GetColorBitboard(us) | _board->GetColorBitboard(nus)) ^ rookSquare;
		const Bitboard rankPieces = pieces & rank;
		const Bitboard filePieces = pieces & file;
		Bitboard squares;

		// -< Down >-
		squares = 0xFFFFFFFFFFFFFFFF << 63 - BitHelper::MsbIdx(filePieces & selfDivisorDown);
		result |= squares & file & selfDivisorDown;

		// -< Up >- 
		squares = 0xFFFFFFFFFFFFFFFF >> 63 - BitHelper::LsbIdx(filePieces & selfDivisorUp);
		result |= squares & file & selfDivisorUp;
		
		// -< Left >-
		squares = 0xFFFFFFFFFFFFFFFF << 63 - BitHelper::MsbIdx(rankPieces & selfDivisorDown);
		result |= squares & rank & selfDivisorDown;

		// -< Right >-
		squares = 0xFFFFFFFFFFFFFFFF >> 63 - BitHelper::LsbIdx(rankPieces & selfDivisorUp);
		result |= squares & rank & selfDivisorUp;

		return result;
	}

	void MoveGen::GeneratePseudoLegalQueenMoves(const Square idx, Color us, std::vector<Move>* movelist)
	{
		GeneratePseudoLegalBishopMoves(idx, us, movelist);
		GeneratePseudoLegalRookMoves(idx, us, movelist);
	}

	Bitboard MoveGen::GenerateQueenAttacks(const Square idx, Color us)
	{
		return GenerateBishopAttacks(idx, us) | GenerateRookAttacks(idx, us);
	}

	void MoveGen::GeneratePseudoLegalKingMoves(const Square idx, Color us, std::vector<Move>* movelist)
	{
		Color nus = Color(!us);
		auto rankIdx = idx / 8;
		auto fileIdx = idx % 8;
		Bitboard allies = _board->GetColorBitboard(us);
		Bitboard enemies = _board->GetColorBitboard(nus);
		Bitboard pieces = allies | enemies;
		Bitboard nusAttacks = _board->GetAttacks(nus);
		Bitboard moves, attacks, result;

		auto ranks = RankMask[rankIdx];
		if (rankIdx < 7) ranks |= RankMask[rankIdx + 1];
		if (rankIdx > 0) ranks |= RankMask[rankIdx - 1];

		auto files = FileMask[fileIdx];
		if (fileIdx < 7) files |= FileMask[fileIdx + 1];
		if (fileIdx > 0) files |= FileMask[fileIdx - 1];

		// Exclude allies and the king himself
		result = files & ranks;
		moves = result ^ (result & pieces);
		attacks = result & enemies;

		// Add bitboard moves
		Square square;
		while ((square = BitHelper::PopLsbIdx(&moves)) != 64) {
			movelist->push_back(MoveHelper::Create(idx, square, MoveHelper::QUIET_MOVE_FLAG));
		}
		while ((square = BitHelper::PopLsbIdx(&attacks)) != 64) {
			movelist->push_back(MoveHelper::Create(idx, square, MoveHelper::CAPTURE_FLAG));
		}

		// King side castling
		if (_board->GetCastlingRights(us, true) && !((pieces | nusAttacks) & 0x60ULL << us * 56)) {
			movelist->push_back(MoveHelper::Create(idx, Misc::SquareIndex0(rankIdx, FileTable::G), MoveHelper::KING_CASTLE_FLAG));
		}

		// Queen side castling
		if (_board->GetCastlingRights(us, false) && !((pieces | nusAttacks) & 0xCULL << us * 56)) {
			movelist->push_back(MoveHelper::Create(idx, Misc::SquareIndex0(rankIdx, FileTable::C), MoveHelper::QUEEN_CASTLE_FLAG));
		}
	}

	Bitboard MoveGen::GenerateKingAttacks(const Square idx, Color us)
	{
		auto rankIdx = idx / 8;
		auto fileIdx = idx % 8;

		auto ranks = RankMask[rankIdx];
		if (rankIdx < 7) ranks |= RankMask[rankIdx + 1];
		if (rankIdx > 0) ranks |= RankMask[rankIdx - 1];

		auto files = FileMask[fileIdx];
		if (fileIdx < 7) files |= FileMask[fileIdx + 1];
		if (fileIdx > 0) files |= FileMask[fileIdx - 1];

		return (files & ranks) ^ (1ULL << idx);
	}


	std::vector<Move> MoveGen::GenerateLegalMoves()
	{
		std::vector<Move> legals;

		for (auto move : GeneratePseudoLegalMoves()) {
			_board->MakeMove(&move, false);
			if (!_board->IsInCheck()) legals.push_back(move);
			_board->UndoMove(&move, false);
		}

		return legals;
	}

}