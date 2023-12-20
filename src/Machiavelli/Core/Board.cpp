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

	void Board::Print()
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

	bool Board::IsInCheck()
	{
		return IsInCheck(GetTurn());
	}

	bool Board::IsInCheck(Color color)
	{
		const Bitboard kingLocation = _pieceColors[color] & _pieceTypes[PieceType::King];
		const Bitboard enemyAttacks = GetAttacks(Color(!color));
		return enemyAttacks & kingLocation;
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

	Board::~Board()
	{
	}

	Board::Board()
	{
		_ply = 0;
		_enPassantSquare = -1;
		_enPassantBitboard = 0;
		_kingCastle[0] = _kingCastle[1] = false;
		_queenCastle[0] = _queenCastle[1] = false;
		BB_FILL(_pieceTypes, 7);
		BB_FILL(_pieceColors, 2);
	}

	Board Board::FromFEN(std::string fen) 
	{
		Board board;

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
			for (auto& piecechar : board.PieceChars) {
				if (piecechar.second == tokens[0].at(i)) {
					p = piecechar.first;
					break;
				}
			}

			// evaluate
			int idx = squareIdx ^ 7;
			board.SetPiece(idx, p);

			squareIdx--;
		}

		// turn
		if (tokens[1].find('w') != std::string::npos) {
			board.SetTurn(Color::White);
		}
		else {
			board.SetTurn(Color::Black);
		}

		// castling
		if (tokens[2].find('K') != std::string::npos) board.SetKingCastlingRights(Color::White, true);
		if (tokens[2].find('Q') != std::string::npos) board.SetQueenCastlingRights(Color::White, true);
		if (tokens[2].find('k') != std::string::npos) board.SetKingCastlingRights(Color::Black, true);
		if (tokens[2].find('q') != std::string::npos) board.SetQueenCastlingRights(Color::Black, true);

		// en passant
		if (tokens[3].find('-') == std::string::npos) {
			board.SetEnpassant(Misc::ToSquareIndex(&tokens[3]));
		}

		// plys for 50 move rule
		if (tokens.size() > 4) {
			board._plys50 = stoi(tokens[4]);
		}

		if (tokens.size() > 5) {
			// TODO? 
			// store move count that the fen gives
		}

		return board;
	}

	void Board::ChangeTurn()
	{
		_turn = Color(!_turn);
	}
}