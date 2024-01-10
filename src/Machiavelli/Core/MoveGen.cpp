#include "pch.h"

#include <iostream>
#include <algorithm>

#include "MoveGen.h"
#include "BitHelper.h"
#include "Misc.h"
#include "MoveHelper.h"


namespace Machiavelli {

	const Bitboard MoveGen::FileMask[8]{
		0x0101010101010101ULL << 0,
		0x0101010101010101ULL << 1,
		0x0101010101010101ULL << 2,
		0x0101010101010101ULL << 3,
		0x0101010101010101ULL << 4,
		0x0101010101010101ULL << 5,
		0x0101010101010101ULL << 6,
		0x0101010101010101ULL << 7,
	};


	const Bitboard MoveGen::RankMask[8]{
		0b11111111ULL << 8 * 0,
		0b11111111ULL << 8 * 1,
		0b11111111ULL << 8 * 2,
		0b11111111ULL << 8 * 3,
		0b11111111ULL << 8 * 4,
		0b11111111ULL << 8 * 5,
		0b11111111ULL << 8 * 6,
		0b11111111ULL << 8 * 7,
	};


	/// <summary> /// Diagonals parallel to (A1 to H8) /// </summary>
	const Bitboard MoveGen::DiagMask1[15]{
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


	/// <summary> /// Diagonals parallel to (A8 to H1) /// </summary>
	const Bitboard MoveGen::DiagMask2[15]{
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



	MoveGen::MoveGen(Board* b)
	{
		_board = b;
	}

	MoveGen::~MoveGen()
	{

	}


	int MoveGen::Perft(int depth, const bool pv, const bool uci)
	{
		if (depth == 0)
			return 1;

		unsigned long long moveCount = 0;
		auto movelist = GenerateLegalMovesConfident();
		for (int i = 0; i < movelist.size(); i++) {
			_board->MakeMove(&movelist[i]);
			int count = Perft(depth - 1, false, uci);
			if (pv) {
				if (uci) 
					// Output using the `info` response format
					std::cout << "info"
						<< " currmove " << MoveHelper::ToString(movelist[i])
						<< " nodes " << count
						<< '\n';
				else 
					// Output custom format
					std::cout << MoveHelper::ToString(movelist[i]) << ": " << count << "\n";

			}
			moveCount += count;
			_board->UndoMove(&movelist[i]);
		}

		return moveCount;
	}

	std::vector<Move> MoveGen::GeneratePseudoLegalMoves()
	{
		return GeneratePseudoLegalMoves(_board->GetTurn());
	}

	std::vector<Move> MoveGen::GenerateLegalMovesConfident() 
	{
		auto pseudos = GeneratePseudoLegalMoves();
		std::vector<Move> legals;
		const Color turn = _board->GetTurn();

		for (auto move : pseudos) {
			_board->MakeMove(&move, false);
			if (_board->GetCheck() == Check::None) legals.push_back(move);
			_board->UndoMove(&move, false);
		}

		return legals;
	}

	std::vector<Move> MoveGen::GenerateLegalMoves()
	{
		std::vector<Move> legals;
		const Color turn = _board->GetTurn();


		// 1. If in double check, generate only valid king moves.
		if (_board->GetCheck() == Check::Double) {
			Square kingIdx = _board->GetPieceBitboard(PieceType::King) & _board->GetColorBitboard(turn);
			GenerateLegalKingMoves_Check(kingIdx, turn, &legals);
			return legals;
		}


		// 2. If in check, generate only check blocking moves.
		if (_board->GetCheck() == Check::Single) {
			GenerateLegalMoves_Check(turn, &legals);
			return legals;
		}


		// 3. If not in check, just don't generate moves for pinned pieces.
		if (_board->GetCheck() == Check::None) {
			GenerateLegalMoves(turn, &legals);
			return legals;
		}
	}

	std::vector<Move> MoveGen::GenerateLegalMoves(Color c)
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

	void MoveGen::GenerateLegalMoves_Check(Color c, std::vector<Move>* movelist)
	{
		auto pieces = _board->GetColorBitboard(c);
		auto square = Square(-1);
		while ((square = BitHelper::PopLsbIdx(&pieces)) != 64) {
			switch (PieceType((_board->GetPiece(square) & Piece::TypeMask) >> 1))
			{
			default: throw std::invalid_argument("Invalid piece type");
			case PT_NULL: break;
			case Pawn: GenerateLegalPawnMoves_Check(square, c, movelist); break;
			case Knight: GenerateLegalKnightMoves_Check(square, c, movelist); break;
			case Bishop: GenerateLegalBishopMoves_Check(square, c, movelist); break;
			case Rook: GenerateLegalRookMoves_Check(square, c, movelist); break;
			case Queen: GenerateLegalQueenMoves_Check(square, c, movelist); break;
			case King: GenerateLegalKingMoves_Check(square, c, movelist); break;
			}
		}
	}

	void MoveGen::GenerateLegalMoves(Color c, std::vector<Move>* movelist)
	{
		auto pieces = _board->GetColorBitboard(c);
		auto square = Square(-1);
		while ((square = BitHelper::PopLsbIdx(&pieces)) != 64) {
			if (_board->GetPinnedPieces() & square) {
				continue;
			}
			switch (PieceType((_board->GetPiece(square) & Piece::TypeMask) >> 1))
			{
			default: throw std::invalid_argument("Invalid piece type");
			case PT_NULL: break;
			case Pawn: GeneratePseudoLegalPawnMoves(square, c, movelist); break;
			case Knight: GeneratePseudoLegalKnightMoves(square, c, movelist); break;
			case Bishop: GeneratePseudoLegalBishopMoves(square, c, movelist); break;
			case Rook: GeneratePseudoLegalRookMoves(square, c, movelist); break;
			case Queen: GeneratePseudoLegalQueenMoves(square, c, movelist); break;
			case King: GeneratePseudoLegalKingMoves(square, c, movelist); break;
			}
		}
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

	void MoveGen::GenerateLegalPawnMoves(const Square idx, Color us, std::vector<Move>* movelist)
	{
	}

	void MoveGen::GeneratePseudoLegalPawnMoves(const Square idx, Color us, std::vector<Move>* movelist) {
		Color nus = Color(!us);

		auto rankIdx = idx / 8;
		auto fileIdx = idx % 8;

		Bitboard enemies = _board->GetColorBitboard(nus);
		Bitboard friends = _board->GetColorBitboard(us);

		const Square captureLeftIdx = idx + (us == Color::White ? 7 : -9);
		const Square captureRightIdx = idx + (us == Color::White ? 9 : -7);
		
		const Rank firstRank = us ? 6 : 1;
		const Rank lastRank = us ? 1 : 6;

		// Move 1 square forward
		// We don't need to check wether the pawn is on the last rank or not, which might cause overflow,
		// as the pawn can never be on the last rank. If it is moved there, it will have to promote.
		Square forward1Idx = idx + (us ? -8 : 8);
		Bitboard forward1 = 1ULL << forward1Idx; // By definition this has always only one set bit
		if (!((enemies | friends) & forward1)) {
			if (rankIdx == lastRank) {
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
			Square forward2Idx = idx + (us ? -16 : 16);
			Bitboard forward2 = 1ULL << forward2Idx; // By definition this has always only one set bit
			if (rankIdx == firstRank && !((enemies | friends) & forward2)) {
				movelist->push_back(MoveHelper::Create(idx, forward2Idx, MoveHelper::DOUBLE_PAWN_PUSH_FLAG));
			}
		}

		// Capture left 
		Bitboard captureLeft = 1ULL << captureLeftIdx;
		if (fileIdx > 0 && enemies & captureLeft) {
			if (rankIdx == lastRank) {
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
			if (rankIdx == lastRank) {
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
	void MoveGen::GenerateLegalPawnMoves_Check(const Square idx, Color us, std::vector<Move>* movelist)
	{
		// Dont move if the pawn is pinned
		if ((1ULL << idx) & _board->GetPinnedPieces()) {
			return;
		}

		Color nus = Color(!us);

		auto rankIdx = idx / 8;
		auto fileIdx = idx % 8;

		Bitboard enemies = _board->GetColorBitboard(nus);
		Bitboard friends = _board->GetColorBitboard(us);

		const Square captureLeftIdx = idx + (us == Color::White ? 7 : -9);
		const Square captureRightIdx = idx + (us == Color::White ? 9 : -7);

		const Rank firstRank = us ? 6 : 1;
		const Rank lastRank = us ? 1 : 6;

		// Move 1 square forward
		// We don't need to check wether the pawn is on the last rank or not, which might cause overflow,
		// as the pawn can never be on the last rank. If it is moved there, it will have to promote.
		Square forward1Idx = idx + (us ? -8 : 8);
		Bitboard forward1 = 1ULL << forward1Idx; // By definition this has always only one set bit
		if (!((enemies | friends) & forward1) && (forward1 & _board->GetCheckBlockades())) {
			if (rankIdx == lastRank) {
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
			Square forward2Idx = idx + (us ? -16 : 16);
			Bitboard forward2 = 1ULL << forward2Idx; // By definition this has always only one set bit
			if (rankIdx == firstRank && !((enemies | friends) & forward2) && (forward2 & _board->GetCheckBlockades())) {
				movelist->push_back(MoveHelper::Create(idx, forward2Idx, MoveHelper::DOUBLE_PAWN_PUSH_FLAG));
			}
		}

		// Capture left 
		Bitboard captureLeft = 1ULL << captureLeftIdx;
		if (fileIdx > 0 && enemies & captureLeft && captureLeft & (_board->GetCheckBlockades() | _board->GetCheckers())) {
			if (rankIdx == lastRank) {
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
		if (fileIdx < 7 && enemies & captureRight && captureRight & (_board->GetCheckBlockades() | _board->GetCheckers())) {
			if (rankIdx == lastRank) {
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
		if ((fileIdx < 7 && (enpassantSqr == captureRightIdx) && captureRight & (_board->GetCheckBlockades() | _board->GetCheckers())) ||	// right
			(fileIdx > 0 && (enpassantSqr == captureLeftIdx) && captureLeft & (_board->GetCheckBlockades() | _board->GetCheckers()))		// left
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
	void MoveGen::GenerateLegalKnightMoves_Check(const Square idx, Color us, std::vector<Move>* movelist)
	{
		// Dont move if the knight is pinned
		if ((1ULL << idx) & _board->GetPinnedPieces()) {
			return;
		}

		const auto rankIdx = idx / 8;
		const auto fileIdx = idx % 8;
		const auto enemies = _board->GetColorBitboard(Color(!us));
		const auto friends = _board->GetColorBitboard(us);
		const auto legal = _board->GetCheckBlockades() | _board->GetCheckers();

		auto addMove = [&](int nidx) {
			Bitboard sqmk = 1ULL << nidx;
			if (!(friends & sqmk) && // no ally on sqmk
				 (legal & sqmk)) { // the move is either check blocking or a capture of a checker
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
	void MoveGen::GenerateLegalBishopMoves_Check(const Square idx, Color us, std::vector<Move>* movelist)
	{
		// Dont move if the bishop is pinned
		if ((1ULL << idx) & _board->GetPinnedPieces()) {
			return;
		}

		Color nus = Color(!us);
		Bitboard moves = GeneratePseudoLegalBishopAttacks(idx, us);
		// If in check, only block or capture the checker
		moves &= _board->GetCheckers() | _board->GetCheckBlockades();
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
	// TODO
	template<CompassRose Direction>
	inline Bitboard MoveGen::GenerateBishopPin(const Square bishopIdx, const Square kingIdx, Color us)
	{
		const Color nus = Color(!us);
		const Bitboard self = 1ULL << bishopIdx;
		const int rankIdx = bishopIdx / 8;
		const int fileIdx = bishopIdx % 8;

		int diagIdx;
		Bitboard bMask, wMask, mask, selfDivisor, diag;
		Bitboard result = 0;

		switch (Direction) {
		case CompassRose::NoEa:
			// -< Up Right >-

			diagIdx = 7 - rankIdx + fileIdx;
			diag = DiagMask1[diagIdx];
			selfDivisor = BitHelper::FromIndex(bishopIdx); // above
			
			bMask = _board->GetColorBitboard(nus) & diag & selfDivisor;
			wMask = (_board->GetColorBitboard(us) >> 9) & (diag & selfDivisor | self);
			mask = bMask | wMask;
			mask &= mask - 1; // remove lsb ()
			result = (0xFFFFFFFFFFFFFFFF >> 63 - BitHelper::LsbIdx(mask)) & diag & selfDivisor;

			return result;
		}
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

	Bitboard MoveGen::GenerateBishopRawConnection(const Square idx1, const Square idx2)
	{
		const Bitboard identity = (1ULL << idx1) | (1ULL << idx2);

		const int rankIdx1 = idx1 / 8;
		const int fileIdx1 = idx1 % 8;
		const int rankIdx2 = idx2 / 8;
		const int fileIdx2 = idx2 % 8;

		Bitboard diag1_1 = DiagMask1[7 - rankIdx1 + fileIdx1];
		Bitboard diag2_1 = DiagMask2[rankIdx1 + fileIdx1];
		Bitboard diag1_2 = DiagMask1[7 - rankIdx2 + fileIdx2];
		Bitboard diag2_2 = DiagMask2[rankIdx2 + fileIdx2];

		const Bitboard connections = (diag1_1 & diag2_1) | (diag1_2 & diag2_2);

		return connections/* ^ identity*/;
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
	void MoveGen::GenerateLegalRookMoves_Check(const Square idx, Color us, std::vector<Move>* movelist)
	{
		// Dont move if the rook is pinned
		if ((1ULL << idx) & _board->GetPinnedPieces()) {
			return;
		}

		Color nus = Color(!us);
		Bitboard moves = GeneratePseudoLegalRookAttacks(idx, us);
		// If in check, only block or capture the checker
		moves &= _board->GetCheckers() | _board->GetCheckBlockades();
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
	Bitboard MoveGen::GenerateRookRawConnection(const Square idx1, const Square idx2)
	{
		const int rankIdx1 = idx1 / 8;
		const int fileIdx1 = idx1 % 8;
		const int rankIdx2 = idx2 / 8;
		const int fileIdx2 = idx2 % 8;
		return (RankMask[rankIdx1] & RankMask[rankIdx2]) | (FileMask[fileIdx1] & FileMask[fileIdx2]);
	}

	void MoveGen::GeneratePseudoLegalQueenMoves(const Square idx, Color us, std::vector<Move>* movelist)
	{
		GeneratePseudoLegalBishopMoves(idx, us, movelist);
		GeneratePseudoLegalRookMoves(idx, us, movelist);
	}
	void MoveGen::GenerateLegalQueenMoves_Check(const Square idx, Color us, std::vector<Move>* movelist)
	{
		// Dont move if the queen is pinned
		if ((1ULL << idx) & _board->GetPinnedPieces()) {
			return;
		}

		GeneratePseudoLegalBishopMoves(idx, us, movelist);
		GeneratePseudoLegalRookMoves(idx, us, movelist);
	}
	Bitboard MoveGen::GenerateQueenAttacks(const Square idx, Color us)
	{
		return GenerateBishopAttacks(idx, us) | GenerateRookAttacks(idx, us);
	}

	Bitboard MoveGen::GenerateQueenRawConnection(const Square idx1, const Square idx2)
	{
		const int rankIdx1 = idx1 / 8;
		const int fileIdx1 = idx1 % 8;
		const int rankIdx2 = idx2 / 8;
		const int fileIdx2 = idx2 % 8;

		Bitboard diag1_1 = DiagMask1[7 - rankIdx1 + fileIdx1];
		Bitboard diag2_1 = DiagMask2[rankIdx1 + fileIdx1];
		Bitboard diag1_2 = DiagMask1[7 - rankIdx2 + fileIdx2];
		Bitboard diag2_2 = DiagMask2[rankIdx2 + fileIdx2];

		return(RankMask[rankIdx1] & RankMask[rankIdx2]) 
			| (FileMask[fileIdx1] & FileMask[fileIdx2])
			| (diag1_1 & diag2_1) 
			| (diag1_2 & diag2_2);
	}

	void MoveGen::GeneratePseudoLegalKingMoves(const Square idx, Color us, std::vector<Move>* movelist)
	{
		Color nus = Color(!us);
		auto rankIdx = idx / 8;
		auto fileIdx = idx % 8;
		const Bitboard allies = _board->GetColorBitboard(us);
		const Bitboard enemies = _board->GetColorBitboard(nus);
		const Bitboard pieces = allies | enemies;
		const Bitboard nusAttacks = _board->GetAttacks(nus);
		const bool inCheck = nusAttacks & (1ULL << idx);
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
		if (!inCheck && // not in check
			_board->GetCastlingRights(us, true) && // allwoed castle
			!((pieces | nusAttacks) & (0x60ULL << us * 56))) // no pieces blocking the way of the rook or king and king wouldn't move through check
		{
			movelist->push_back(MoveHelper::Create(idx, Misc::SquareIndex0(rankIdx, FileTable::G), MoveHelper::KING_CASTLE_FLAG));
		}

		// Queen side castling
		if (!inCheck && // not in check
			_board->GetCastlingRights(us, false) && // allowed castle
			!(pieces & (0xEULL << us * 56)) && // no pieces blocking the way of the rook or king
			!(nusAttacks & (0xCULL << us * 56))) // king wouldn't move through check
		{
			movelist->push_back(MoveHelper::Create(idx, Misc::SquareIndex0(rankIdx, FileTable::C), MoveHelper::QUEEN_CASTLE_FLAG));
		}
	}
	// Not generating castling moves
	void MoveGen::GenerateLegalKingMoves_Check(const Square idx, Color us, std::vector<Move>* movelist)
	{
		Color nus = Color(!us);
		auto rankIdx = idx / 8;
		auto fileIdx = idx % 8;
		const Bitboard allies = _board->GetColorBitboard(us);
		const Bitboard enemies = _board->GetColorBitboard(nus);
		const Bitboard pieces = allies | enemies;
		const Bitboard nusAttacks = _board->GetAttacks(nus);
		const bool inCheck = nusAttacks & (1ULL << idx);
		Bitboard moves, legalAttacks, result;

		// Create the square of moves
		auto ranks = RankMask[rankIdx];
		if (rankIdx < 7) ranks |= RankMask[rankIdx + 1];
		if (rankIdx > 0) ranks |= RankMask[rankIdx - 1];

		auto files = FileMask[fileIdx];
		if (fileIdx < 7) files |= FileMask[fileIdx + 1];
		if (fileIdx > 0) files |= FileMask[fileIdx - 1];
		
		result = files & ranks;

		// Exclude allies and the king himself
		moves = result ^ (result & pieces);
		legalAttacks = (result & enemies) ^ (result & nusAttacks); // enemies that aren't protected can be captured

		// Add bitboard moves
		Square square;
		while ((square = BitHelper::PopLsbIdx(&moves)) != 64) {
			movelist->push_back(MoveHelper::Create(idx, square, MoveHelper::QUIET_MOVE_FLAG));
		}
		while ((square = BitHelper::PopLsbIdx(&legalAttacks)) != 64) {
			movelist->push_back(MoveHelper::Create(idx, square, MoveHelper::CAPTURE_FLAG));
		}
	}
	void MoveGen::GenerateLegalKingMoves(const Square idx, Color us, std::vector<Move>* movelist)
	{
		Color nus = Color(!us);
		auto rankIdx = idx / 8;
		auto fileIdx = idx % 8;
		const Bitboard allies = _board->GetColorBitboard(us);
		const Bitboard enemies = _board->GetColorBitboard(nus);
		const Bitboard pieces = allies | enemies;
		const Bitboard nusAttacks = _board->GetAttacks(nus);
		const bool inCheck = nusAttacks & (1ULL << idx);
		Bitboard moves, legalAttacks, result;

		// Create the square of moves
		auto ranks = RankMask[rankIdx];
		if (rankIdx < 7) ranks |= RankMask[rankIdx + 1];
		if (rankIdx > 0) ranks |= RankMask[rankIdx - 1];

		auto files = FileMask[fileIdx];
		if (fileIdx < 7) files |= FileMask[fileIdx + 1];
		if (fileIdx > 0) files |= FileMask[fileIdx - 1];

		result = files & ranks;

		// Exclude allies and the king himself
		moves = result ^ (result & pieces);
		legalAttacks = (result & enemies) ^ (result & nusAttacks); // enemies that aren't protected can be captured

		// Add bitboard moves
		Square square;
		while ((square = BitHelper::PopLsbIdx(&moves)) != 64) {
			movelist->push_back(MoveHelper::Create(idx, square, MoveHelper::QUIET_MOVE_FLAG));
		}
		while ((square = BitHelper::PopLsbIdx(&legalAttacks)) != 64) {
			movelist->push_back(MoveHelper::Create(idx, square, MoveHelper::CAPTURE_FLAG));
		}

		// King side castling
		if (!inCheck && // not in check
			_board->GetCastlingRights(us, true) && // allwoed castle
			!((pieces | nusAttacks) & (0x60ULL << us * 56))) // no pieces blocking the way of the rook or king and king wouldn't move through check
		{
			movelist->push_back(MoveHelper::Create(idx, Misc::SquareIndex0(rankIdx, FileTable::G), MoveHelper::KING_CASTLE_FLAG));
		}

		// Queen side castling
		if (!inCheck && // not in check
			_board->GetCastlingRights(us, false) && // allowed castle
			!(pieces & (0xEULL << us * 56)) && // no pieces blocking the way of the rook or king
			!(nusAttacks & (0xCULL << us * 56))) // king wouldn't move through check
		{
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



}