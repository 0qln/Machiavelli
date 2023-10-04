#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <list>
#include <algorithm>
#include <cstdint>
#include <vector>

#include "Misc.hpp"
#include "FenHelper.hpp"
#include "Move.hpp"


struct Board {

private:

	int _ply = 0;

	int _move = 0;

	bool _turn = true;

	bool _whiteKingCasle = false;
	bool _whiteQueenCasle = false;
	bool _blackKingCasle = false;
	bool _blackQueenCasle = false;

	Bitboard _whitePieces = 0, _blackPieces = 0;
	Bitboard _pawns = 0, _knights = 0, _bishops = 0, _rooks = 0, _queens = 0, _kings = 0;
	Bitboard _enPassant = 0;


	//char PieceChars[13]{
	//	'.',
	//	'♙', '♘', '♗', '♖', '♕', '♔', //whhite
	//	'♟', '♞', '♝', '♜', '♛', '♚', //black
	//};
	std::map<Piece, char> PieceChars = {
		std::make_pair(Piece::None, '.'),

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

	Board() { }

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
		Square squareIdx = 0;
		int fenIdx = parts[0];
		for (; fenIdx < parts[1]; fenIdx++) {
			/// skip slashes
			if (fen.at(fenIdx) == '/') {
				continue;
			}

			/// handle digit
			if (isdigit(fen.at(fenIdx))) {	
				squareIdx += (fen.at(fenIdx) - '0');
				continue;
			}

			/// get piece char as Piece Type
			Piece p = Piece::None;
			for (auto& i : PieceChars) {
				if (i.second == fen.at(fenIdx)) {
					p = i.first;
					break; 
				}
			}


			/// evaluate
			if ((p & Piece::TypeMask) == Piece::Pawn) 
				FlipBit(&_pawns, squareIdx);
			if ((p & Piece::TypeMask) == Piece::Knight) 
				FlipBit(&_knights, squareIdx);
			if ((p & Piece::TypeMask) == Piece::Bishop) 
				FlipBit(&_bishops, squareIdx);
			if ((p & Piece::TypeMask) == Piece::Rook) 
				FlipBit(&_rooks, squareIdx);
			if ((p & Piece::TypeMask) == Piece::Queen) 
				FlipBit(&_queens, squareIdx);
			if ((p & Piece::TypeMask) == Piece::King) 
				FlipBit(&_kings, squareIdx);
					
			if ((p & Piece::ColorMask) == Piece::White) 
				FlipBit(&_whitePieces, squareIdx);
			if ((p & Piece::ColorMask) == Piece::Black) 
				FlipBit(&_blackPieces, squareIdx);
					
			squareIdx++;
		}
		
		// turn
		str = fen.substr(parts[1], parts[2] - parts[1]);
		if (str.find('w') != std::string::npos) {
			_turn = true;
		}
		else {
			_turn = false;
		}

		// castling
		str = fen.substr(parts[2], parts[3] - parts[2]);
		if (str.find('K') != std::string::npos) {
			_whiteKingCasle = true;
		}
		if (str.find('Q') != std::string::npos) {
			_whiteQueenCasle = true;
		}
		if (str.find('k') != std::string::npos) {
			_blackKingCasle = true;
		}
		if (str.find('q') != std::string::npos) {
			_blackQueenCasle = true;
		}

		// en passant
		str = fen.substr(parts[3], parts[4] - parts[3]);
		if (str.find('-') != std::string::npos) {
			FlipBit(&_enPassant, ToSquareIndex(str));
		}


		// ply
		str = fen.substr(parts[4]+1, parts[5] - parts[4]);
		_ply = stoi(str);

		// move
		str = fen.substr(parts[5]+1, fen.size() - parts[5]);
		_move = stoi(str);
	}


	void GenerateMoves(Move* movelist) {
		*movelist++ = Move(0);
	}

	// just assumes the move parameter is a legal one.
	void MakeMove(Move* move) {
		Square from = MoveHelper::GetFrom(move);
		Square to = MoveHelper::GetTo(move);
		auto flag = MoveHelper::GetFlag(move);
		Piece movingP = PieceAt(&from);

		if (flag == MoveHelper::CAPTURE_FLAG) {
			// remove piece
		}

	}

	// just assumes the move parameter is a legal one.
	void UndoMove(Move* move) {
	}


	int Perft() {
		
	}


	// receives for example 'e3'
	int ToSquareIndex(std::string square) {
		return SquareIndex(square.at(1), square.at(0) - 'b');
	}

	void FlipBit(Bitboard* board, Square squareIdx) {
		*board |= (Bitboard(1) << squareIdx);
	}

	Piece PieceAt(Square* index) {
		Bitboard sm = SquareMask(index);
		int piece = Piece::None;

		if		((_pawns	& sm) != 0)	piece |= Piece::Pawn;
		else if ((_knights	& sm) != 0)	piece |= Piece::Knight;
		else if ((_bishops	& sm) != 0)	piece |= Piece::Bishop;
		else if ((_rooks	& sm) != 0)	piece |= Piece::Rook;
		else if ((_queens	& sm) != 0)	piece |= Piece::Queen;
		else if ((_kings	& sm) != 0)	piece |= Piece::King;

		if		(IsWhite(index)) piece |= Piece::White;
		else if (IsBlack(index)) piece |= Piece::Black;

		return Piece(piece);
	}

	bool IsWhite(Square* index) {
		return (_whitePieces & SquareMask(index)) != 0;
	}

	bool IsBlack(Square* index) {
		return (_blackPieces & SquareMask(index)) != 0;
	}


	inline Bitboard SquareMask(Square* index) {
		return Bitboard(1) << *index;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="row">1-indexed</param>
	/// <param name="file">1-indexed</param>
	/// <returns>0-indexed</returns>
	Square SquareIndex(int row, int file) {
		return file + 8 * (row-1) - 1;
	}


	std::string ToSring() {
		std::stringstream ss;


		ss << "  _______________\n";
		Square squareIdx = 0;
		for (int row = 1; row <= 8; row++) {
			ss << row << " ";

			for (int file = 1; file <= 8; file++) {
				ss << PieceChars[PieceAt(&squareIdx)] << " ";
				++squareIdx;
			}

			ss << "\n";
		}
		ss << "  " << "a b c d e f g h";


		std::string ret = ss.str();
		return ret;
	}
};

