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
	const static Piece nullPiece = Piece(0);

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
		Square squareIdx = 63;
		for (int fenIdx = parts[0]; fenIdx < parts[1]; fenIdx++) {
			/// skip slashes
			if (fen.at(fenIdx) == '/') {
				continue;
			}

			/// handle digit
			if (isdigit(fen.at(fenIdx))) {	
				squareIdx -= (fen.at(fenIdx) - '0');
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
			SetPiece(&squareIdx, &p);
					

			squareIdx--;
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


	void GenerateMoves(Move* movelist) {

	}

	// just assumes the move parameter is a legal one.
	void MakeMove(const Move* move) {
		const Square from = MoveHelper::GetFrom(move);
		const Square to = MoveHelper::GetTo(move);
		const MoveHelper::Flag flag = MoveHelper::GetFlag(move);
		const Piece movingP = GetPiece(&from);


		// handle castling
		if (MoveHelper::IsCastle(move)) {
			// apply castling rules
			Move kingMove;
			Move rookMove;
			int rank = _turn ? /*white*/ 1 : /*black*/ 8;
			if (MoveHelper::IsKingSideCastle(move)) {
				// init moves
				kingMove = MoveHelper::Create(Misc::SquareIndex(rank, 5), Misc::SquareIndex(rank, 7), MoveHelper::Flags::QUITE_MOVE_FLAG);
				rookMove = MoveHelper::Create(Misc::SquareIndex(rank, 8), Misc::SquareIndex(rank, 6), MoveHelper::Flags::QUITE_MOVE_FLAG);
			}
			if (MoveHelper::IsQueenSideCastle(move)) {
				// init moves
				kingMove = MoveHelper::Create(Misc::SquareIndex(rank, 5), Misc::SquareIndex(rank, 3), MoveHelper::Flags::QUITE_MOVE_FLAG);
				rookMove = MoveHelper::Create(Misc::SquareIndex(rank, 1), Misc::SquareIndex(rank, 4), MoveHelper::Flags::QUITE_MOVE_FLAG);
			}
			MakeMove(&kingMove);
			MakeMove(&rookMove);

			return;
		}
		
		// handle captures
		if (MoveHelper::IsCapture(move)) {
			if (MoveHelper::IsEnPassantCapture(move)) {
				int removeRank = 3 + _turn;  //0-indexed
				int removeFile = to % 8;	 //0-indexed
				Square removeIdx = removeFile + 8 * removeRank;

				// remove piece with offset...
				SetPiece(&removeIdx, &nullPiece);

				// remove possible en passant capture
				_enPassant = 0;
			}
			else {
				// remove piece at `to` Square
				SetPiece(&to, &nullPiece);
			}
		}

		
		// handle promotion
		Piece newP;
		if (MoveHelper::IsPromotion(move)) {
			int type = MoveHelper::IsPromotionWithCapture(move) ? flag * 4 - 40 : flag * 4 - 24;
			int color = movingP & Piece::ColorMask;
			newP = Piece((type << 2) | color);
		}
		else {
			newP = movingP;
		}


		// move the piece
		SetPiece(&to, &newP);
		SetPiece(&from, &nullPiece);


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


	int Perft() const {
		
	}

	inline void DeactivateBit(Bitboard* board, const Square* squareIdx) {
		/*ActivateBit(board, squareIdx);
		FlipBit(board, squareIdx);*/
		SetBit(board, squareIdx, 0);
	}
	inline void ActivateBit(Bitboard* board, const Square* squareIdx) {
		*board |= (Bitboard(1) << *squareIdx);
	}
	inline void FlipBit(Bitboard* board, const Square* squareIdx) {
		*board ^= (Bitboard(1) << *squareIdx);
	}
	inline void SetBit(Bitboard* board, const Square* squareIdx, bool value) {
		//*board |= (Bitboard(1) << *squareIdx); // default to an active bit
		//*board ^= (Bitboard(!value) << *squareIdx); // flip the bit if needed
		*board = (*board | (Bitboard(1) << *squareIdx)) ^ (Bitboard(!value) << *squareIdx);
	}

	void SetPiece(const Square* squareIdx, const Piece* p) {
		
		if ((*p & Piece::TypeMask) == Piece::Pawn)
			 ActivateBit(&_pawns, squareIdx);
		else DeactivateBit(&_pawns, squareIdx);

		
		if ((*p & Piece::TypeMask) == Piece::Knight)
			 ActivateBit(&_knights, squareIdx);
		else DeactivateBit(&_knights, squareIdx);


		if ((*p & Piece::TypeMask) == Piece::Bishop)
		 	 ActivateBit(&_bishops, squareIdx);
		else DeactivateBit(&_bishops, squareIdx);


		if ((*p & Piece::TypeMask) == Piece::Rook)
			 ActivateBit(&_rooks, squareIdx);
		else DeactivateBit(&_rooks, squareIdx);


		if ((*p & Piece::TypeMask) == Piece::Queen)
			 ActivateBit(&_queens, squareIdx);
		else DeactivateBit(&_queens, squareIdx);


		if ((*p & Piece::TypeMask) == Piece::King)
			 ActivateBit(&_kings, squareIdx);
		else DeactivateBit(&_kings, squareIdx);


		if ((*p & Piece::ColorMask) == Piece::White)
			 ActivateBit(&_whitePieces, squareIdx);
		else DeactivateBit(&_whitePieces, squareIdx);


		if ((*p & Piece::ColorMask) == Piece::Black)
			 ActivateBit(&_blackPieces, squareIdx);
		else DeactivateBit(&_blackPieces, squareIdx);
	}

	Piece GetPiece(const Square* index) const {
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

	inline bool IsWhite(const Square* index) const {
		return (_whitePieces & SquareMask(index)) != 0;
	}

	inline bool IsBlack(const Square* index) const {
		return (_blackPieces & SquareMask(index)) != 0;
	}


	inline Bitboard SquareMask(const Square* index) const {
		return Bitboard(1) << *index;
	}



	std::string ToSring() {
		std::stringstream ss;


		ss << "  _______________\n";
		Square squareIdx = 0;
		for (int rank = 1; rank <= 8; rank++) {
			ss << rank << " ";

			for (int file = 1; file <= 8; file++) {
				ss << PieceChars[GetPiece(&squareIdx)] << " ";
				++squareIdx;
			}

			ss << "\n";
		}
		ss << "  " << "a b c d e f g h";


		std::string ret = ss.str();
		return ret;
	}
};

