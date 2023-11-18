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

#include "Misc.hpp"
#include "Move.hpp"

class Board {


private:

	int _ply = 0;

	int _move = 0;

	bool _turn = Color::White;

	bool _kingCasle[2];
	bool _queenCasle[2];

	Bitboard _checkers = 0, _pinners = 0;
	Bitboard _pieceTypes[7]; 
	Bitboard _pieceColors[2];
	Bitboard _enPassant = 0;


	//char PieceChars[13]{
	//	'.',
	//	'♙', '♘', '♗', '♖', '♕', '♔', //white
	//	'♟', '♞', '♝', '♜', '♛', '♚', //black
	//};
	std::map<Piece, char> PieceChars = {
		std::make_pair(Piece::P_NULL, '.'),

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


public:

	Bitboard GetColorBitboard(Color color) {
		return _pieceColors[color];
	}

	Bitboard GetPieceBitboard(PieceType type) {
		return _pieceTypes[type];
	}

	bool Turn() { return _turn; }


	static inline void Flip(Bitboard* board) {
		*board ^= 56;
	}

	Board(std::string fen) {
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
			Piece p = Piece::P_NULL;
			for (auto& i : PieceChars) {
				if (i.second == fen.at(fenIdx)) {
					p = i.first;
					break; 
				}
			}

			/// evaluate
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
		if (str.find('K') != std::string::npos) _kingCasle[Color::White] = true;
		if (str.find('Q') != std::string::npos) _queenCasle[Color::White] = true;
		if (str.find('k') != std::string::npos) _kingCasle[Color::Black] = true;
		if (str.find('q') != std::string::npos) _queenCasle[Color::Black] = true;
		

		// en passant
		str = fen.substr(parts[3], parts[4] - parts[3]);
		if (str.find('-') == std::string::npos) {
			Square idx = Misc::ToSquareIndex(&str);
			ActivateBit(&_enPassant, &idx);
		}


		// ply
		str = fen.substr(parts[4]+1, parts[5] - parts[4]);
		_ply = stoi(str);


		// move
		str = fen.substr(parts[5]+1, fen.size() - parts[5]);
		_move = stoi(str);
	}






	// just assumes the move parameter is a legal one.
	void MakeMove(const Move* move) {
		// handle castling
		if (MoveHelper::IsCastle(move)) {
			// apply castling rules
			Move kingMove, rookMove;
			int rank = _turn == Color::White ? Rank(0) : Rank(7);
			// init moves
			if (MoveHelper::IsKingSideCastle(move)) {
				kingMove = MoveHelper::Create(rank + FileTable::E, rank + FileTable::G, MoveHelper::QUITE_MOVE_MASK);
				rookMove = MoveHelper::Create(rank + FileTable::H, rank + FileTable::F, MoveHelper::QUITE_MOVE_MASK);
				_kingCasle[_turn] = false;
			}
			if (MoveHelper::IsQueenSideCastle(move)) {
				kingMove = MoveHelper::Create(rank + FileTable::E, rank + FileTable::C, MoveHelper::QUITE_MOVE_MASK);
				rookMove = MoveHelper::Create(rank + FileTable::A, rank + FileTable::D, MoveHelper::QUITE_MOVE_MASK);
				_queenCasle[_turn] = false;
			}

			MakeMove(&kingMove);
			MakeMove(&rookMove);

			return;
		}

		
		const Square from = MoveHelper::GetFrom(move);
		const Square to = MoveHelper::GetTo(move);
		const MoveHelper::Flag flag = MoveHelper::GetFlag(move);
		const Piece movingP = GetPiece(from);


		// castling rights
		if (movingP >> 1 == PieceType::Rook && from == SquareTable::A1) _queenCasle[Color::White] = false;
		if (movingP >> 1 == PieceType::Rook && from == SquareTable::H1) _kingCasle [Color::White] = false;
		if (movingP >> 1 == PieceType::Rook && from == SquareTable::A8) _queenCasle[Color::Black] = false;
		if (movingP >> 1 == PieceType::Rook && from == SquareTable::H8) _kingCasle [Color::Black] = false;
		if (movingP >> 1 == PieceType::King) {
			_queenCasle[_turn] = false;
			_kingCasle [_turn] = false;
		}

		// handle captures
		if (MoveHelper::IsCapture(move)) {
			if (MoveHelper::IsEnPassantCapture(move)) {
				int removeRank = 3 + !_turn;
				int removeFile = to % 8;
				Square removeIdx = removeFile + 8 * removeRank;

				// remove piece with offset...
				SetPiece(removeIdx, Misc::NullPiece);

				// remove possible en passant capture
				_enPassant = 0;
			}
			else {
				// remove piece at `to` Square
				SetPiece(to, Misc::NullPiece);
			}
		}

		
		// handle promotion
		Piece newP;
		if (MoveHelper::IsPromotion(move)) {
			int type = MoveHelper::IsPromotionWithCapture(move) ? flag - 4 : flag;
			int color = !_turn;
			newP = Piece((type << 1) | color);
		}
		else {
			newP = movingP;
		}


		// move the piece
		SetPiece(from, Piece::P_NULL);
		SetPiece(to, newP);


		if (MoveHelper::IsDoublePawnPush(move)) {
			// remember as a possible en passant capture
			// ex: 
			//		e4 (-remember e3)
			//		e5 (-remember e6)
			
			int rank = 2 + !_turn * 3;  //0-indexed
			int file = to % 8;			//0-indexed
			Square sq = file + rank * 8;
			ActivateBit(&_enPassant, &sq);
		}

		// change turn
		_turn = !_turn;
	}

	// just assumes the move parameter is a legal one.
	void UndoMove(const Move* move) {
	
	}



	inline void DeactivateBit(Bitboard* board, const Square* squareIdx) {
		*board &= ~(1ULL << *squareIdx);
	}
	inline void ActivateBit(Bitboard* board, const Square* squareIdx) {
		*board |= (1ULL << *squareIdx);
	}
	inline void FlipBit(Bitboard* board, const Square* squareIdx) {
		*board ^= (1ULL << *squareIdx);
	}
	inline void FlipBits(Bitboard* board, const Square* squareIdx1, const Square* squareIdx2) {
		*board ^= (1ULL << *squareIdx1 | 1ULL << *squareIdx2);
	}
	inline bool GetBit(Bitboard board, const Square* squareIdx) {
		return (board >> *squareIdx) & 1;
	}
	inline void SetBit(Bitboard* board, const Square* squareIdx, const bool value) {
		*board = (*board & (1ULL << *squareIdx)) | *board << *squareIdx; 
	}


	void SetPiece(const Square squareIdx, const Piece p) {

		for (Bitboard typeBoard : _pieceTypes) {
			// each square can only be occupied by one piece type
			DeactivateBit(&typeBoard, &squareIdx);
		}
		ActivateBit(&_pieceTypes[(p >> 1)], &squareIdx);


		if ((p & Piece::ColorMask) == Color::White) 
			 ActivateBit(&_pieceColors[Color::White], &squareIdx);
		else DeactivateBit(&_pieceColors[Color::White], &squareIdx);


		if ((p & Piece::ColorMask) == Color::Black)
			 ActivateBit(&_pieceColors[Color::Black], &squareIdx);
		else DeactivateBit(&_pieceColors[Color::Black], &squareIdx);
	}

	Piece GetPiece(const Square index) const {
		Bitboard sm = SquareMask(&index);
		int piece = Piece::P_NULL;

		for (int bi = PieceType::PT_NULL; bi <= PieceType::King; bi++) {
			if ((_pieceTypes[bi] & sm) != 0) {
				piece |= PieceType(bi) << 1;
				break;
			}
		}

		piece |= IsBlack(&index);

		return Piece(piece);
	}


	inline bool IsWhite(const Square* index) const {
		return (_pieceColors[Color::White] & SquareMask(index)) != 0;
	}
	inline bool IsBlack(const Square* index) const {
		return (_pieceColors[Color::Black] & SquareMask(index)) != 0;
	}

	inline Bitboard SquareMask(const Square* index) const {
		return Bitboard(1) << *index;
	}



	std::string ToSring(Bitboard highlightSquares) {
		std::stringstream ss;


		ss << "   _______________\n";
		for (Rank rank = 7; rank >= 0; rank--) {
			ss << rank + 1 << "  ";

			for (File file = FileTable::A; file <= FileTable::H; file++) {
				int idx = Misc::SquareIndex_0IDX(rank, file);
				auto p = GetPiece(idx);
				char c = PieceChars[p]; 
				if (/*c == PieceChars[Piece::P_NULL] &&*/
					GetBit(highlightSquares, &idx)) {
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

