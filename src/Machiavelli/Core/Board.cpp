#include "pch.h"

#include "BitHelper.h"
#include "Board.h"
#include "Misc.h"
#include "MoveHelper.h"

#include <algorithm>
#include <array>
#include <assert.h>
#include <bit>
#include <bitset>
#include <cstdint>
#include <cstdint>
#include <iostream>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <regex>


#define BB_FILL(n, s) for (int i = 0; i < s; i++) n[i] = 0; 


namespace Machiavelli {

	const std::map<Piece, char> Board::PieceChars = {
		std::make_pair(Piece::WhiteNULL,	'.'),
		std::make_pair(Piece::BlackNULL,	'.'),

		std::make_pair(Piece::WhitePawn,	'P'),
		std::make_pair(Piece::WhiteKnight,	'N'),
		std::make_pair(Piece::WhiteBishop,	'B'),
		std::make_pair(Piece::WhiteRook,	'R'),
		std::make_pair(Piece::WhiteQueen,	'Q'),
		std::make_pair(Piece::WhiteKing,	'K'),

		std::make_pair(Piece::BlackPawn,	'p'),
		std::make_pair(Piece::BlackKnight,	'n'),
		std::make_pair(Piece::BlackBishop,	'b'),
		std::make_pair(Piece::BlackRook,	'r'),
		std::make_pair(Piece::BlackQueen,	'q'),
		std::make_pair(Piece::BlackKing,	'k'),
	};

	void Board::MakeMove(const Move* move, bool changeTurn)
	{
		const Square from = MoveHelper::GetFrom(move);
		const Square to = MoveHelper::GetTo(move);
		const Piece movingPiece = GetPiece(from);
		const int fRankIdx = from / 8;
		const MoveHelper::Flag flag = MoveHelper::GetFlag(move);
		Piece newP = movingPiece;
		const Color us = Color(GetPiece(from) & Piece::ColorMask);

		// Remember what happened
		BoardState newState;
		newState.move = *move;
		newState.ply = ++_ply;
		newState.capturedSquare = to;
		newState.capturedPiece = GetPiece(to);
		newState.kingCastle[Color::White] = _kingCastle[Color::White];
		newState.kingCastle[Color::Black] = _kingCastle[Color::Black];
		newState.queenCastle[Color::White] = _queenCastle[Color::White];
		newState.queenCastle[Color::Black] = _queenCastle[Color::Black];

		// remove old en passant
		ResetEnpassant();

		// handle extra cases
		switch (MoveHelper::GetFlag(move))
		{
		case MoveHelper::KING_CASTLE_FLAG:
			SetPiece(fRankIdx * 8 + FileTable::E, Piece::WhiteNULL);
			SetPiece(fRankIdx * 8 + FileTable::G, Piece((PieceType::King << 1) | us));
			SetPiece(fRankIdx * 8 + FileTable::H, Piece::WhiteNULL);
			SetPiece(fRankIdx * 8 + FileTable::F, Piece((PieceType::Rook << 1) | us));
			_kingCastle[us] = false;
			goto finish;
		case MoveHelper::QUEEN_CASTLE_FLAG:
			SetPiece(fRankIdx * 8  + FileTable::E, Piece::WhiteNULL);
			SetPiece(fRankIdx * 8  + FileTable::C, Piece((PieceType::King << 1) | us));
			SetPiece(fRankIdx * 8  + FileTable::A, Piece::WhiteNULL);
			SetPiece(fRankIdx * 8  + FileTable::D, Piece((PieceType::Rook << 1) | us));
			_queenCastle[us] = false;
			goto finish;

		case MoveHelper::EN_PASSANT_FLAG:
			// give offset to `removeIdx`
			newState.capturedSquare += (us * 2 - 1) * 8;
			// get capture piece (has to be a pawn)
			newState.capturedPiece = Piece(!us | (PieceType::Pawn << 1));
			// clear the square of the pawn that got en passanted
			SetPiece(newState.capturedSquare, Piece::WhiteNULL);
			// do the normal capturing procedure:
			goto capture;

		case MoveHelper::CAPTURE_FLAG:
capture:
			// remove the ability to castle when the rooks are captured
			switch (to)
			{
			case SquareTable::A1: _queenCastle[Color::White] = false; break;
			case SquareTable::H1: _kingCastle[Color::White] = false; break;
			case SquareTable::A8: _queenCastle[Color::Black] = false; break;
			case SquareTable::H8: _kingCastle[Color::Black] = false; break;
			}
			break;

		case MoveHelper::DOUBLE_PAWN_PUSH_FLAG:
			SetEnpassant(to + (us * 2 - 1) * 8);
			break;

			// Swap out piece on promotion
		case MoveHelper::PROMOTION_KNIGHT_FLAG:
		case MoveHelper::PROMOTION_BISHOP_FLAG:
		case MoveHelper::PROMOTION_ROOK_FLAG:
		case MoveHelper::PROMOTION_QUEEN_FLAG:
			newP = Piece((flag << 1) | us);
			break;
		case MoveHelper::CAPTURE_PROMOTION_KNIGHT_FLAG:
		case MoveHelper::CAPTURE_PROMOTION_BISHOP_FLAG:
		case MoveHelper::CAPTURE_PROMOTION_ROOK_FLAG:
		case MoveHelper::CAPTURE_PROMOTION_QUEEN_FLAG:
			newP = Piece((flag - 4 << 1) | us);
			goto capture;
		}

		// move the piece
		SetPiece(from, Piece::WhiteNULL);
		SetPiece(to, newP);


finish:
		// remove castling rights on movement of the rook or king
		if (movingPiece >> 1 == PieceType::Rook) {
			switch (from)
			{
			case SquareTable::A1: _queenCastle[Color::White] = false; break;
			case SquareTable::H1: _kingCastle[Color::White] = false; break;
			case SquareTable::A8: _queenCastle[Color::Black] = false; break;
			case SquareTable::H8: _kingCastle[Color::Black] = false; break;
			}
		}
		else if (movingPiece >> 1 == PieceType::King) {
			_queenCastle[us] = false;
			_kingCastle[us] = false;
			//std::cout << "remove castle rights" << '\n';
		}

		// change turn
		if (changeTurn) {
			ChangeTurn();
		}


		// Update state definitions
		Update();


	  	_boardStates.push_back(&newState);
	}

	void Board::UndoMove(const Move* move, bool changeTurn)
	{
		const auto currentState = _boardStates[_ply-1];
		const Square from = MoveHelper::GetFrom(move);
		const Square to = MoveHelper::GetTo(move);
		Piece movingPiece = GetPiece(to);
		Piece capturedPiece = currentState.capturedPiece;
		Square capturedIdx = to;
		const int fRankIdx = from / 8;
		const Color us = Color(GetPiece(to) & Piece::ColorMask);

		switch (MoveHelper::GetFlag(move))
		{
		case MoveHelper::KING_CASTLE_FLAG:
			SetPiece(fRankIdx * 8 + FileTable::G, Piece::WhiteNULL);
			SetPiece(fRankIdx * 8 + FileTable::E, Piece((PieceType::King << 1) | us));
			SetPiece(fRankIdx * 8 + FileTable::F, Piece::WhiteNULL);
			SetPiece(fRankIdx * 8 + FileTable::H, Piece((PieceType::Rook << 1) | us));
			_kingCastle[us] = true;
			goto finish;
		case MoveHelper::QUEEN_CASTLE_FLAG:
			SetPiece(fRankIdx * 8  + FileTable::C, Piece::WhiteNULL);
			SetPiece(fRankIdx * 8  + FileTable::E, Piece((PieceType::King << 1) | us));
			SetPiece(fRankIdx * 8  + FileTable::D, Piece::WhiteNULL);
			SetPiece(fRankIdx * 8  + FileTable::A, Piece((PieceType::Rook << 1) | us));
			_queenCastle[us] = true;
			goto finish;

		case MoveHelper::CAPTURE_PROMOTION_QUEEN_FLAG:
		case MoveHelper::CAPTURE_PROMOTION_KNIGHT_FLAG:
		case MoveHelper::CAPTURE_PROMOTION_ROOK_FLAG:
		case MoveHelper::CAPTURE_PROMOTION_BISHOP_FLAG:
		case MoveHelper::PROMOTION_QUEEN_FLAG:
		case MoveHelper::PROMOTION_KNIGHT_FLAG:
		case MoveHelper::PROMOTION_ROOK_FLAG:
		case MoveHelper::PROMOTION_BISHOP_FLAG:
			movingPiece = Piece(us | (PieceType::Pawn << 1));
			break;

		case MoveHelper::EN_PASSANT_FLAG:
			capturedPiece = Piece(!us | (PieceType::Pawn << 1));
			capturedIdx = to + (us * 2 - 1) * 8;
			break;

		case MoveHelper::DOUBLE_PAWN_PUSH_FLAG:
			SetEnpassant(to + (us * 2 - 1) * 8);
		}

		// Move piece
		SetPiece(to, Piece::WhiteNULL);
		SetPiece(capturedIdx, capturedPiece);
		SetPiece(from, movingPiece);

finish:
		// castling rights
		_kingCastle[0] = currentState.kingCastle[0];
		_kingCastle[1] = currentState.kingCastle[1];
		_queenCastle[0] = currentState.queenCastle[0];
		_queenCastle[1] = currentState.queenCastle[1];

		// Change turn
		if (changeTurn) {
			ChangeTurn();
		}

		_boardStates.pop_back();

		// Update state definitions
		// TODO: replace this with theb definitions being cached in the board state
		Update();

		--_ply;
	}

	Piece Board::GetPiece(const Square index)
	{
		Bitboard b = 1ULL << index;
		int c = 0 != (_pieceColors[Color::Black] & b);
		for (int i = PieceType::Pawn; i <= PieceType::King; ++i) {
			if (_pieceTypes[i] & b) return Piece((i << 1) | c);
		}
		return Piece::WhiteNULL;
	}

	void Board::SetPiece(const Square index, const Piece piece)
	{
		for (int i = PieceType::Pawn; i <= PieceType::King; ++i) {
			// each square can only be occupied by one piece type, so we don't
			// need to check wether they are set or not
			BitHelper::DeactivateBit(&_pieceTypes[i], &index);
		}

		// handle not-NULL-pieces
		if (piece >> 1) {
			// activate piece type
			BitHelper::ActivateBit(&_pieceTypes[piece >> 1], &index);

			// change colors
			BitHelper::ActivateBit(&_pieceColors[piece & Piece::ColorMask], &index);
			BitHelper::DeactivateBit(&_pieceColors[!(piece & Piece::ColorMask)], &index);
		}
		// handle colors for NULL-pieces
		else {
			BitHelper::DeactivateBit(&_pieceColors[Color::White], &index);
			BitHelper::DeactivateBit(&_pieceColors[Color::Black], &index);
		}
	}

	std::string Board::ToString(Bitboard highlightSquares)
	{
		std::stringstream ss;

		//ss << "   _______________\n";
		for (Rank rank = 7; rank >= 0; rank--) {
			ss << rank + 1 << "  ";

			for (File file = FileTable::A; file <= FileTable::H; file++) {
				int idx = Misc::SquareIndex0(rank, file);
				auto p = GetPiece(idx);
				char c = PieceChars.at(p);
				if (/*c == PieceChars[Piece::P_NULL] &&*/
					BitHelper::GetBit(highlightSquares, &idx)) {
					c = 'x';
				}

				ss << c << " ";
			}

			ss << "\n";
		}
		ss << "   " << "a b c d e f g h" << "\n";

		return ss.str();
	}

	void Board::ResetEnpassant()
	{
		_enPassantSquare = -1;
		_enPassantBitboard = 0ULL;
	}

	void Board::SetEnpassant(Square idx)
	{
		_enPassantSquare = idx;
		_enPassantBitboard = 1ULL << idx;
	}

	void Board::PrintBoard()
	{
		std::cout << ToString() << '\n';
	}

	void Board::PrintPieceTypeBitboard(PieceType pt)
	{
		std::cout << "Piece type: " << pt << '\n';
		BitHelper::PrintBitboard(GetPieceBitboard(pt));
	}

	void Board::PrintPieceTypeBitboard(Piece p)
	{
		PrintPieceTypeBitboard(PieceType(p >> 1));
	}

	void Board::PrintColorBitboard(Color c)
	{
		std::cout << "Color: " << c << '\n';
		BitHelper::PrintBitboard(GetColorBitboard(c));
	}

	void Board::PrintColorBitboard(Piece p)
	{
		PrintColorBitboard(Color(p & Piece::ColorMask));
	}

	void Board::PrintPieceBitboard(PieceType pt, Color c)
	{
		std::cout << "Piece type: " << pt << " - Color: " << c << '\n';
		BitHelper::PrintBitboard(GetPieceBitboard(pt) & GetColorBitboard(c));
	}

	void Board::PrintPieceBitboard(Piece p)
	{
		PrintPieceBitboard(PieceType(p >> 1), Color(p & Piece::ColorMask));
	}

	void Board::PrintCastlingRights() 
	{
		std::string crights;
		if (GetCastlingRights(Color::White, true)) crights.append("K");
		if (GetCastlingRights(Color::White, false)) crights.append("Q");
		if (GetCastlingRights(Color::Black, true)) crights.append("k");
		if (GetCastlingRights(Color::Black, false)) crights.append("q");
		std::cout << (crights == "" ? "-" : crights) << '\n';
	}

	Bitboard Board::GetAttacks(Color color)
	{
		auto pieces = GetColorBitboard(color);
		auto result = Bitboard();
		auto square = Square();
		while ((square = BitHelper::PopLsbIdx(&pieces)) != 64) {
			result |= MoveGen::MoveGen(this).GenerateAttacks(square, color);
		}
		return result;
	}

	bool Board::GetCastlingRights(Color color, bool kingSide)
	{
		return kingSide ? _kingCastle[color] : _queenCastle[color];
	}

	Bitboard Board::GetColorBitboard(Color color)
	{
		return _pieceColors[color];
	}

	Bitboard Board::GetPieceBitboard(PieceType type)
	{
		return _pieceTypes[type];
	}

	Bitboard Board::GetEnPassantBitboard()
	{
		return _enPassantBitboard;
	}

	Square Board::GetEnPassantSquare()
	{
		return _enPassantSquare;
	}

	void Board::SetTurn(Color turn)
	{
		_turn = turn;
	}

	void Board::SetQueenCastlingRights(Color c, bool value)
	{
		_queenCastle[c] = value;
	}

	void Board::SetKingCastlingRights(Color c, bool value)
	{
		_kingCastle[c] = value;
	}

	Color Board::GetTurn()
	{
		return _turn;
	}

	Check Board::GetCheck() {
		return _checkState < 2 ? Check(_checkState) : Check::Double;
	}

	void Board::Update()
	{
		// reset states
		_checkState = Check::None;
		_checkers = 0;
		_checkBlockades = 0;
		BB_FILL(_attacks, 2);
		BB_FILL(_pinnedPieces, 2);

		// local variables
		const Color nus = Color(!_turn), us = _turn;
		const Bitboard kingLocationUs = _pieceColors[us] & _pieceTypes[PieceType::King];
		const Bitboard kingLocationNus = _pieceColors[nus] & _pieceTypes[PieceType::King];
		const Square kingSquareNus = BitHelper::LsbIdx(kingLocationNus);
		const Square kingSquareUs = BitHelper::LsbIdx(kingLocationUs);
		Bitboard enemies = _pieceColors[nus], allies = _pieceColors[us];
		Square square = Square();

		// Iterate enemies
		while ((square = BitHelper::PopLsbIdx(&enemies)) != 64) {
			const auto attacks = MoveGen::MoveGen(this).GenerateAttacks(square, nus);
			
			// Update enemy attacks
			_attacks[nus] |= attacks;
			
			// Checking
			if (attacks & kingLocationUs) {
				// -> New check detected
				_checkers |= 1ULL << square;
				++_checkState;
				
				// Update possible check blocks, if the enemy on `square` is a sliding piece
				PieceType pt = PieceType(GetPiece(square) >> 1);
				Bitboard other_otherlike, king_otherlike;

				switch (pt)
				{
				case PieceType::Rook:
					other_otherlike = MoveGen::MoveGen(this).GenerateRookRaw(square);
					king_otherlike = MoveGen::MoveGen(this).GenerateRookRaw(kingSquareUs);
					break;
				case PieceType::Bishop:
					other_otherlike = MoveGen::MoveGen(this).GenerateBishopRaw(square);
					king_otherlike = MoveGen::MoveGen(this).GenerateBishopRaw(kingSquareUs);
					break;
				case PieceType::Queen:
					other_otherlike = MoveGen::MoveGen(this).GenerateQueenRaw(square);
					king_otherlike = MoveGen::MoveGen(this).GenerateQueenRaw(kingSquareUs);
					break;
				default:
					goto continue_after_check;
				}
				const auto king_other_view = other_otherlike & king_otherlike;
				const auto king_divisor = BitHelper::FromIndex(kingSquareUs);
				const auto other_divisor = BitHelper::FromIndex(square);
				Bitboard divisor = 0;
				switch (square - kingSquareUs > 0) {
				case false: divisor = ~king_divisor & other_divisor; break;
				case true: divisor = king_divisor & ~other_divisor; break;
				}
				const auto king_other_between = king_other_view & divisor;
				_checkBlockades |= king_other_between;
			}
continue_after_check:

			// Update pinned allies
			PieceType pt = PieceType(GetPiece(square) >> 1);
			Bitboard other_otherlike, king_otherlike;

			switch (pt) {
			case PieceType::Rook:
				other_otherlike = MoveGen::MoveGen(this).GenerateRookRaw(square);
				king_otherlike = MoveGen::MoveGen(this).GenerateRookRaw(kingSquareUs);
				break;
			case PieceType::Bishop:
				other_otherlike = MoveGen::MoveGen(this).GenerateBishopRaw(square);
				king_otherlike = MoveGen::MoveGen(this).GenerateBishopRaw(kingSquareUs);
				break;
			case PieceType::Queen:
				other_otherlike = MoveGen::MoveGen(this).GenerateQueenRaw(square);
				king_otherlike = MoveGen::MoveGen(this).GenerateQueenRaw(kingSquareUs);
				break;
			default:
				continue;
			}

			const auto king_rook_view = other_otherlike & king_otherlike;
			const auto king_divisor = BitHelper::FromIndex(kingSquareUs);
			const auto other_divisor = BitHelper::FromIndex(square);
			Bitboard divisor = 0;
			switch (square - kingSquareUs > 0) {
			case false: divisor = ~king_divisor & other_divisor; break;
			case true: divisor = king_divisor & ~other_divisor; break;
			}
			const auto king_other_between = king_rook_view & divisor;
			const auto pieces_between = king_other_between & _pieceColors[us];
			if (BitHelper::CountBits(pieces_between) == 1)
				_pinnedPieces[us] |= pieces_between;
		}

		// Iterate allies
		square = Square();
		while ((square = BitHelper::PopLsbIdx(&allies)) != 64) {
			const auto attacks = MoveGen::MoveGen(this).GenerateAttacks(square, us);
			// Update ally attacks
			_attacks[us] |= attacks;

			// Update pinned enemies
			PieceType pt = PieceType(GetPiece(square) >> 1);
			Bitboard other_otherlike, king_otherlike;

			switch (pt) {
			case PieceType::Rook:
				other_otherlike = MoveGen::MoveGen(this).GenerateRookRaw(square);
				king_otherlike = MoveGen::MoveGen(this).GenerateRookRaw(kingSquareNus);
				break;
			case PieceType::Bishop:
				other_otherlike = MoveGen::MoveGen(this).GenerateBishopRaw(square);
				king_otherlike = MoveGen::MoveGen(this).GenerateBishopRaw(kingSquareNus);
				break;
			case PieceType::Queen:
				other_otherlike = MoveGen::MoveGen(this).GenerateQueenRaw(square);
				king_otherlike = MoveGen::MoveGen(this).GenerateQueenRaw(kingSquareNus);
				break;
			default:
				continue;
			}

			const auto king_other_view = other_otherlike & king_otherlike;
			const auto king_divisor = BitHelper::FromIndex(kingSquareNus);
			const auto other_divisor = BitHelper::FromIndex(square);
			Bitboard divisor = 0;
			switch (square - kingSquareNus > 0) {
			case false: divisor = ~king_divisor & other_divisor; break;
			case true: divisor = king_divisor & ~other_divisor; break;
			}
			const auto king_other_between = king_other_view & divisor;
			const auto pieces_between = king_other_between & _pieceColors[nus];
			if (BitHelper::CountBits(pieces_between) == 1)
				_pinnedPieces[nus] |= pieces_between;
		}
	}

	Bitboard Board::GetCheckBlockades()
	{
		return _checkBlockades;
	}

	Bitboard Board::GetPinnedPieces(Color color)
	{
		return _pinnedPieces[color];
	}
	Bitboard Board::GetPinnedPieces()
	{
		return GetPinnedPieces(GetTurn());
	}

	Bitboard Board::GetCheckers() 
	{
		return _checkers;
	}

	Board::~Board()
	{
	}

	Board::Board()
	{
		_checkState = Check::None;
		_ply = 0;
		_enPassantSquare = -1;
		_enPassantBitboard = 0;
		_kingCastle[0] = _kingCastle[1] = false;
		_queenCastle[0] = _queenCastle[1] = false;
		BB_FILL(_pieceTypes, 7);
		BB_FILL(_pieceColors, 2);
		BB_FILL(_pinnedPieces, 2);
		BB_FILL(_attacks, 2);
		_checkers = 0;
		_checkBlockades = 0;
	}

	Board Board::FromFEN(std::string fen) 
	{
		Board board;

		board.SetUpFEN(fen);

		return board;
	}

	// assuming the board is cleared
	void Board::SetUpFEN(std::string fen) {

		std::vector<std::string> tokens;
		std::regex re("\\S+");
		std::sregex_token_iterator begin(fen.begin(), fen.end(), re), end;
		std::copy(begin, end, std::back_inserter(tokens));

		// Set up pieces
		Square squareIdx = 63;
		for (int i = 0; i < tokens[0].size(); i++) {
			// skip slashes
			if (tokens[0].at(i) == '/') {
				continue;
			}

			// handle digit
			if (isdigit(tokens[0].at(i))) {
				squareIdx -= tokens[0].at(i) - '0';
				continue;
			}

			// get piece char as Piece Type
			Piece p = Piece::WhiteNULL;
			for (auto& piecechar : PieceChars) {
				if (piecechar.second == tokens[0].at(i)) {
					p = piecechar.first;
					break;
				}
			}

			// evaluate
			int idx = squareIdx ^ 7;
			SetPiece(idx, p);

			squareIdx--;
		}

		// turn
		if (tokens[1].find('w') != std::string::npos) {
			SetTurn(Color::White);
		}
		else {
			SetTurn(Color::Black);
		}

		// castling
		if (tokens[2].find('K') != std::string::npos) SetKingCastlingRights(Color::White, true);
		if (tokens[2].find('Q') != std::string::npos) SetQueenCastlingRights(Color::White, true);
		if (tokens[2].find('k') != std::string::npos) SetKingCastlingRights(Color::Black, true);
		if (tokens[2].find('q') != std::string::npos) SetQueenCastlingRights(Color::Black, true);

		// en passant
		if (tokens[3].find('-') == std::string::npos) {
			SetEnpassant(Misc::ToSquareIndex(&tokens[3]));
		}

		// plys for 50 move rule
		if (tokens.size() > 4) {
			_plys50 = stoi(tokens[4]);
		}

		if (tokens.size() > 5) {
			// TODO? 
			// store move count that the fen gives
		}

		Update();
	}

	void Board::ChangeTurn()
	{
		_turn = Color(!_turn);
	}
}