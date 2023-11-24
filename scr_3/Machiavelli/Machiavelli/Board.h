#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <list>
#include <algorithm>
#include <cstdint>
#include <vector>
#include <array>
#include <bit>
#include <bitset>
#include <cstdint>
#include <iostream>
#include <assert.h>


#include "Misc.h"
#include "Move.h"
#include "BitHelper.h"
#include "MoveGen.h"


class MoveGen;


struct BoardState {
	BoardState* previous;
	
	Piece capturedPiece;
	Square capturedSquare;

	Move move;
	
	int ply;

public:
	BoardState() { 
		previous = nullptr;
		capturedPiece = Piece(0);
		capturedSquare = Square(-1);
		move = Move(0);
		ply = -1;
	}
};


class Board {
private:

	int _ply = 0;

	int _move = 0;

	Color _turn = Color::White;

	bool _kingCastle[2];
	bool _queenCastle[2];

	//Bitboard _checkers, _pinners;
	Bitboard _attackers[64];
	Bitboard _pieceTypes[7]; 
	Bitboard _pieceColors[2];
	Bitboard _enPassantBitboard;
	Square _enPassantSquare;

	BoardState _currentState;

	//char PieceChars[13]{
	//	'.',
	//	'♙', '♘', '♗', '♖', '♕', '♔', //white
	//	'♟', '♞', '♝', '♜', '♛', '♚', //black
	//}; ??
	std::map<Piece, char> PieceChars = {
		std::make_pair(Piece::WhiteNULL, '.'),
		std::make_pair(Piece::BlackNULL, '.'),

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


	void SetState(BoardState* newState) {
		newState->previous = new BoardState(_currentState);
		_currentState = *newState;
	}

	void PopState() {
		_currentState = *_currentState.previous;
	}

	void SetEnpassant(Square idx) {
		_enPassantSquare = idx;
		_enPassantBitboard = 1ULL << idx;
	}

	void ResetEnpassant() {
		_enPassantSquare = -1;
		_enPassantBitboard = 0ULL;
	}

public:

	bool GetCastlingRights(Color color, bool kingSide) {
		return kingSide ? _kingCastle[color] : _queenCastle[color];
	}

	Bitboard GetColorBitboard(Color color) {
		return _pieceColors[color];
	}

	Bitboard GetPieceBitboard(PieceType type) {
		return _pieceTypes[type];
	}

	Bitboard GetEnPassantBitboard() {
		return _enPassantBitboard;
	}

	Square GetEnPassantSquare() {
		return _enPassantSquare;
	}

	void SetTurn(Color turn) {
		_turn = turn;
	}

	Color GetTurn() { 
		return _turn; 
	}

	inline bool IsInCheck() { return IsInCheck(GetTurn()); }
	inline bool IsInCheck(Color color) {
		//return _attacksColors[!color] & _pieceTypes[PieceType::King] & _pieceColors[color];
	}

	// Return a bitboard of all squares that can be used to resolve the current check
	inline Bitboard GetCheckBlockades() { return GetCheckBlockades(GetTurn()); }
	inline Bitboard GetCheckBlockades(Color color) {
		if (!IsInCheck(color)) {
			return ~0ULL;
		}


	}


	Board(std::string fen) {
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
			/// skip slashes
			if (fen.at(fenIdx) == '/') {
				continue;
			}

			/// handle digit
			if (isdigit(fen.at(fenIdx))) {	
				squareIdx -= fen.at(fenIdx) - '0';
				continue;
			}

			/// get piece char as Piece Type
			Piece p = Piece::WhiteNULL;
			for (auto& i : PieceChars) {
				if (i.second == fen.at(fenIdx)) {
					p = i.first;
					break; 
				}
			}

			/// evaluate
			int idx = squareIdx ^ 7;
			SetPiece(idx, p);
			//MoveGen gen = MoveGen::MoveGen(this);

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
		if (str.find('K') != std::string::npos) _kingCastle [Color::White] = true;
		if (str.find('Q') != std::string::npos) _queenCastle[Color::White] = true;
		if (str.find('k') != std::string::npos) _kingCastle [Color::Black] = true;
		if (str.find('q') != std::string::npos) _queenCastle[Color::Black] = true;
		

		// en passant
		str = fen.substr(parts[3], parts[4] - parts[3]);
		if (str.find('-') == std::string::npos) {
			SetEnpassant(Misc::ToSquareIndex(&str));
		}


		// ply
		str = fen.substr(parts[4]+1, parts[5] - parts[4]);
		_ply = stoi(str);


		// move
		str = fen.substr(parts[5]+1, fen.size() - parts[5]);
		_move = stoi(str);
	}



	// just assumes the move parameter is a legal one.
	void MakeMove(const Move* move, bool changeTurn = true) {
		const Square from = MoveHelper::GetFrom(move); 
		const Square to = MoveHelper::GetTo(move); 
		const Piece movingPiece = GetPiece(from);
		const int fRankIdx = from / 8;
		const MoveHelper::Flag flag = MoveHelper::GetFlag(move);

		
		// Remember what happened
		BoardState newState = BoardState::BoardState();
		newState.move = *move;
		newState.ply = ++_ply;
		newState.capturedSquare = to;
		newState.capturedPiece = GetPiece(to);


		// handle castling
		if (MoveHelper::IsKingSideCastle(move)) {
			SetPiece(fRankIdx + FileTable::E, Piece::WhiteNULL);
			SetPiece(fRankIdx + FileTable::G, Piece((PieceType::King << 1) | _turn));
			SetPiece(fRankIdx + FileTable::H, Piece::WhiteNULL);
			SetPiece(fRankIdx + FileTable::F, Piece((PieceType::Rook << 1) | _turn));
			_kingCastle[_turn] = false;
			return;
		}
		else if (MoveHelper::IsQueenSideCastle(move)) {
			SetPiece(fRankIdx + FileTable::E, Piece::WhiteNULL);
			SetPiece(fRankIdx + FileTable::C, Piece((PieceType::King << 1) | _turn));
			SetPiece(fRankIdx + FileTable::A, Piece::WhiteNULL);
			SetPiece(fRankIdx + FileTable::D, Piece((PieceType::Rook << 1) | _turn));
			_queenCastle[_turn] = false;
			return;
		}		


		// castling rights
		if (movingPiece >> 1 == PieceType::Rook && from == SquareTable::A1) _queenCastle[Color::White] = false;
		if (movingPiece >> 1 == PieceType::Rook && from == SquareTable::H1) _kingCastle [Color::White] = false;
		if (movingPiece >> 1 == PieceType::Rook && from == SquareTable::A8) _queenCastle[Color::Black] = false;
		if (movingPiece >> 1 == PieceType::Rook && from == SquareTable::H8) _kingCastle [Color::Black] = false;
		if (movingPiece >> 1 == PieceType::King) { _queenCastle[_turn] = false; _kingCastle [_turn] = false; }


		// handle captures
		if (MoveHelper::IsCapture(move)) {
			
			// if the rook's gone, we can no longer castle
			if (to == SquareTable::A1) { _queenCastle[Color::White] = false; }
			if (to == SquareTable::H1) { _kingCastle[Color::White] = false; }
			if (to == SquareTable::A8) { _queenCastle[Color::Black] = false; }
			if (to == SquareTable::H8) { _kingCastle[Color::Black] = false; }

			// handle en passant capture
			if (MoveHelper::IsEnPassantCapture(move)) {
				// give offset to `removeIdx`
				newState.capturedSquare += (_turn * 2 - 1) * 8;
				// get capture piece (has to be a pawn)
				newState.capturedPiece = Piece(!_turn | (PieceType::Pawn << 1));
				// clear the square of the pawn that got en passanted
				SetPiece(newState.capturedSquare, Piece::WhiteNULL);
			}
		}

		
		// handle promotion
		Piece newP = movingPiece; 
		if (MoveHelper::IsPromotion(move)) {
			int type = MoveHelper::IsPromotionWithCapture(move) ? flag - 4 : flag;
			newP = Piece((type << 1) | _turn);
		}


		// move the piece
		SetPiece(from, Piece::WhiteNULL);
		SetPiece(to, newP);


		// remove old en passant
		ResetEnpassant();
		// remember as possible en passant capture, if possible
		if (MoveHelper::IsDoublePawnPush(move)) {		
			SetEnpassant(to + (_turn * 2 - 1) * 8);
		}


		// change turn
		if (changeTurn) {
			ChangeTurn();
		}

		SetState(&newState);
	}

	inline void ChangeTurn() {
		_turn = Color(!_turn);
	}

	// just assumes the move parameter is a legal one.
	void UndoMove(const Move* move, bool changeTurn = true) {
		const Square from = MoveHelper::GetFrom(move);
		const Square to = MoveHelper::GetTo(move);
		Piece movingPiece = GetPiece(to);
		Piece capturedPiece = _currentState.capturedPiece;
		Square capturedIdx = to;
		const int fRankIdx = from / 8;


		// handle castling
		if (MoveHelper::IsKingSideCastle(move)) {
			// king 
			SetPiece(fRankIdx + FileTable::G, Piece::WhiteNULL); 
			SetPiece(fRankIdx + FileTable::E, Piece((PieceType::King << 1) | _turn));

			// rook
			SetPiece(fRankIdx + FileTable::F, Piece::WhiteNULL);
			SetPiece(fRankIdx + FileTable::H, Piece((PieceType::Rook << 1) | _turn));

			_kingCastle[_turn] = true;
			return;
		}
		else if (MoveHelper::IsQueenSideCastle(move)) {
			// king 
			SetPiece(fRankIdx + FileTable::C, Piece::WhiteNULL);
			SetPiece(fRankIdx + FileTable::E, Piece((PieceType::King << 1) | _turn));

			// rook
			SetPiece(fRankIdx + FileTable::D, Piece::WhiteNULL);
			SetPiece(fRankIdx + FileTable::A, Piece((PieceType::Rook << 1) | _turn));

			_queenCastle[_turn] = true;
			return;
		}

		
		// handle promotion
		if (MoveHelper::IsPromotion(move)) {
			movingPiece = Piece(_turn | (PieceType::Pawn << 1));
		}


		// en passant capture
		if (MoveHelper::IsEnPassantCapture(move)) {
			capturedPiece = Piece(!_turn | (PieceType::Pawn << 1));
			capturedIdx = to + (_turn * 2 - 1) * 8;
		}


		// double pawn push
		ResetEnpassant();
		if (MoveHelper::IsDoublePawnPush(move)) {
			SetEnpassant(to + (_turn * 2 - 1) * 8);
		}


		// Move piece
		SetPiece(to, Piece::WhiteNULL);
		SetPiece(capturedIdx, capturedPiece);
		SetPiece(from, movingPiece);


		// Change state
		PopState();


		// Change turn
		if (changeTurn) {
			ChangeTurn();
		}

		--_ply;

	}




	void Clear() {
		//_checkers = _pinners = 
		_enPassantBitboard = 0;
		_enPassantSquare = -1;
		_pieceColors[Color::White] = _pieceColors[Color::Black] = 0;
		_pieceTypes[PieceType::Pawn] = _pieceTypes[PieceType::Knight] = _pieceTypes[PieceType::Bishop] = _pieceTypes[PieceType::Rook] = _pieceTypes[PieceType::Queen] = 
			_pieceTypes[PieceType::King] = _pieceTypes[PieceType::PT_NULL] = 0;
	}

	void SetPiece(const Square squareIdx, const Piece p) {
		for (int i = PieceType::Pawn; i <= PieceType::King; ++i) {
			// each square can only be occupied by one piece type, so we don't 
			// need to check wether they are set or not
			BitHelper::DeactivateBit(&_pieceTypes[i], &squareIdx);
		}

		// handle not-NULL-pieces
		if (p >> 1) {
			// activate piece type
			_pieceTypes[p >> 1] |= 1ULL << squareIdx;

			for (int i = Color::White; i <= Color::Black; ++i) {
				if ((p & Piece::ColorMask) == i) {
					BitHelper::ActivateBit(&_pieceColors[i], &squareIdx);
				}
				else {
					BitHelper::DeactivateBit(&_pieceColors[i], &squareIdx);
				}
			}
		}	
		// handle colors for NULL-pieces
		else {
			for (int i = Color::White; i <= Color::Black; ++i) {
				BitHelper::DeactivateBit(&_pieceColors[i], &squareIdx);
			}
		}
	}

	Piece GetPiece(const Square index) const {
		int piece = Piece::WhiteNULL;

		// Get type
		for (int i = PieceType::Pawn; i <= PieceType::King; ++i) {
			if (_pieceTypes[i] & SquareMask(&index)) {
				piece |= PieceType(i) << 1;

				// Get color
				piece |= IsBlack(&index); 

				return Piece(piece); 
			}
		}

		return Piece::WhiteNULL;
	}


	inline bool IsWhite(const Square* index) const {
		return _pieceColors[Color::White] & SquareMask(index);
	}
	inline bool IsBlack(const Square* index) const {
		return _pieceColors[Color::Black] & SquareMask(index);
	}

	inline Bitboard SquareMask(const Square* index) const {
		return 1ULL << *index;
	}


	std::string ToSring(Bitboard highlightSquares = 0) {
		std::stringstream ss;


		ss << "   _______________\n";
		for (Rank rank = 7; rank >= 0; rank--) {
			ss << rank + 1 << "  ";

			for (File file = FileTable::A; file <= FileTable::H; file++) {
				int idx = Misc::SquareIndex_0IDX(rank, file);
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
		ss << "   " << "a b c d e f g h\n";


		return ss.str();
	}
};

