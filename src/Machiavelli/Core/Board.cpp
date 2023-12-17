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


namespace Machiavelli {
	void Board::MakeMove(const Move* move, bool changeTurn)
	{
		const Square from = MoveHelper::GetFrom(move);
		const Square to = MoveHelper::GetTo(move);
		const Piece movingPiece = GetPiece(from);
		const int fRankIdx = from / 8;
		const MoveHelper::Flag flag = MoveHelper::GetFlag(move);
		Piece newP = movingPiece;
		const Color us = Color(GetPiece(from) & Piece::ColorMask);
		/*std::cout 
			<< "Turn: " << us 
			<< " | Move: " << MoveHelper::ToString(*move) << '\n';*/

		// Remember what happened
		BoardState newState = BoardState::BoardState();
		newState.move = *move;
		newState.ply = ++_ply;
		newState.capturedSquare = to;
		newState.capturedPiece = GetPiece(to);
		newState.kingCastle[0] = _kingCastle[0];
		newState.kingCastle[1] = _kingCastle[1];
		newState.queenCastle[0] = _queenCastle[0];
		newState.queenCastle[1] = _queenCastle[1];

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

		SetState(&newState);
	}

	void Board::UndoMove(const Move* move, bool changeTurn)
	{
		const Square from = MoveHelper::GetFrom(move);
		const Square to = MoveHelper::GetTo(move);
		Piece movingPiece = GetPiece(to);
		Piece capturedPiece = _currentState.capturedPiece;
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

		// castling rights
		_kingCastle[0] = _currentState.kingCastle[0];
		_kingCastle[1] = _currentState.kingCastle[1];
		_queenCastle[0] = _currentState.queenCastle[0];
		_queenCastle[1] = _currentState.queenCastle[1];

		// Move piece
		SetPiece(to, Piece::WhiteNULL);
		SetPiece(capturedIdx, capturedPiece);
		SetPiece(from, movingPiece);

		// Change turn
finish:
		if (changeTurn) {
			ChangeTurn();
		}

		// Change state
		PopState();

		--_ply;
	}

	void Board::Clear()
	{
		//_checkers = _pinners =
		_enPassantBitboard = 0;
		_enPassantSquare = -1;
		_pieceColors[Color::White] = _pieceColors[Color::Black] = 0;
		_pieceTypes[PieceType::Pawn] = _pieceTypes[PieceType::Knight] = _pieceTypes[PieceType::Bishop] = _pieceTypes[PieceType::Rook] = _pieceTypes[PieceType::Queen] =
			_pieceTypes[PieceType::King] = _pieceTypes[PieceType::PT_NULL] = 0;
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
				char c = PieceChars[p];
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

	void Board::PopState()
	{
		_currentState = *_currentState.previous;
	}

	void Board::SetState(BoardState* newState)
	{
		newState->previous = new BoardState(_currentState);
		_currentState = *newState;
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

	Color Board::GetTurn()
	{
		return _turn;
	}

	bool Board::IsInCheck()
	{
		return IsInCheck(GetTurn());
	}


	bool Board::IsInCheck(Color color)
	{
		//std::cout << "Board::IsInCheck" << '\n';

		//int i = 0;

		// Go through all lines the enemy can play 
		for (auto move : MoveGen::MoveGen(this).GeneratePseudoLegalMoves(Color(!color)))
		{
			MakeMove(&move, false);

			// If the king was taken in some line, the site must've been in check
			if (!BitHelper::CountBits(GetPieceBitboard(PieceType::King) & GetColorBitboard(color))) {
				UndoMove(&move, false);
				return true;
			}

			//if (i == 37) {
			//	std::cout << MoveHelper::ToString(move) << '\n';
			//	std::cout << ToString() << '\n';
			//}

			UndoMove(&move, false);


			//if (i == 37) break;

			//i++;
		}
		
		//std::cout << ToString() << '\n';

		//std::cout << "Board::IsInCheck" << '\n';

		return false;
	}

	Bitboard Board::GetCheckBlockades()
	{
		return GetCheckBlockades(GetTurn());
	}

	// TODO
	Bitboard Board::GetCheckBlockades(Color color)
	{
		if (!IsInCheck(color)) {
			return ~0ULL;
		}
	}

	Board::Board(std::string fen)
	{
		_currentState = BoardState::BoardState();
		_currentState.ply = 0;
		_enPassantSquare = -1;

		std::string str;

		int parts[6];
		int spaceIdx = 0;
		int partIdx = 0;
		parts[partIdx] = 0;
		while (++spaceIdx < fen.size()) {
			if (fen.at(spaceIdx) != ' ') continue;
			parts[++partIdx] = spaceIdx;
		}

		// Set up pieces
		Square squareIdx = 63;
		for (int fenIdx = parts[0]; fenIdx < parts[1]; fenIdx++) {
			// skip slashes
			if (fen.at(fenIdx) == '/') {
				continue;
			}

			// handle digit
			if (isdigit(fen.at(fenIdx))) {
				squareIdx -= fen.at(fenIdx) - '0';
				continue;
			}

			// get piece char as Piece Type
			Piece p = Piece::WhiteNULL;
			for (auto& i : PieceChars) {
				if (i.second == fen.at(fenIdx)) {
					p = i.first;
					break;
				}
			}

			// evaluate
			int idx = squareIdx ^ 7;
			SetPiece(idx, p);

			squareIdx--;
		}

		// turn
		str = fen.substr(parts[1], parts[2] - parts[1]);
		if (str.find('w') != std::string::npos) {
			_turn = Color::White;
		}
		else {
			_turn = Color::Black;
		}

		// castling
		str = fen.substr(parts[2], parts[3] - parts[2]);
		if (str.find('K') != std::string::npos) _kingCastle[Color::White] = true;
		if (str.find('Q') != std::string::npos) _queenCastle[Color::White] = true;
		if (str.find('k') != std::string::npos) _kingCastle[Color::Black] = true;
		if (str.find('q') != std::string::npos) _queenCastle[Color::Black] = true;

		// en passant
		str = fen.substr(parts[3], parts[4] - parts[3]);
		if (str.find('-') == std::string::npos) {
			SetEnpassant(Misc::ToSquareIndex(&str));
		}

		// optional:
		if (parts[5] >= 0 && parts[5] < fen.size()) {
			// ply
			str = fen.substr(parts[4] + 1, parts[5] - parts[4]);
			_ply = stoi(str);

			// move
			str = fen.substr(parts[5] + 1, fen.size() - parts[5]);
			_move = stoi(str);
		}
	}

	void Board::ChangeTurn()
	{
		_turn = Color(!_turn);
	}
}