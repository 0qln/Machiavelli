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
			if (p) SetPiece(&squareIdx, &p);
					

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

	void GenerateRookMoves(Move* movelist, Bitboard rooks) {

	}

	const Bitboard File[8] {
		0x0101010101010101ULL << 0,
		0x0101010101010101ULL << 1,
		0x0101010101010101ULL << 2,
		0x0101010101010101ULL << 3,
		0x0101010101010101ULL << 4,
		0x0101010101010101ULL << 5,
		0x0101010101010101ULL << 6,
		0x0101010101010101ULL << 7,
	};
	const Bitboard Rank[8]{
		0b11111111ULL << 8 * 0,
		0b11111111ULL << 8 * 1,
		0b11111111ULL << 8 * 2,
		0b11111111ULL << 8 * 3,
		0b11111111ULL << 8 * 4,
		0b11111111ULL << 8 * 5,
		0b11111111ULL << 8 * 6,
		0b11111111ULL << 8 * 7,
	};

	Bitboard RookAttacks(Bitboard rook) {
		Square rookPos = std::countr_zero(rook);
		Bitboard result = Rank[rookPos / 8] | File[rookPos % 8]; 
		return result ^ rook;
	}

	Bitboard RookAttacks() {
		Bitboard tRooks1 = _rooks;
		Bitboard tRooks2 = _rooks;
		while (true) {
			PopLsb(&tRooks2);
			if (!tRooks2) break;
			PopLsb(&tRooks1);
		}
		return RookAttacks(tRooks1); 
	}

	Square PopLsb(Bitboard* board) {
		Square lsb = std::countr_zero(*board);  
		*board &= *board - 1;
		return lsb; 
	}

	Square Msb(Bitboard* board) {
		Square msb = std::countl_zero(*board); 
		return msb;
	}

	// probably slow, should only be used for testing!
	bool ValueAtSquare(Bitboard* board, Square index) {
		return 1 & (*board >> index);
	}

	// just assumes the move parameter is a legal one.
	void MakeMove(const Move* move) {
		// handle castling
		if (MoveHelper::IsCastle(move)) {
			// apply castling rules
			Move kingMove, rookMove;
			int rank = !_turn * 56;
			// init moves
			if (MoveHelper::IsKingSideCastle(move)) {
				kingMove = MoveHelper::Create(rank + 3, rank + 1, 0);
				rookMove = MoveHelper::Create(rank    , rank + 2, 0);
			}
			if (MoveHelper::IsQueenSideCastle(move)) {
				kingMove = MoveHelper::Create(rank + 3, rank + 5, 0);
				rookMove = MoveHelper::Create(rank + 7, rank + 4, 0);
			}
			MakeMove(&kingMove);
			MakeMove(&rookMove);

			return;
		}

		
		const Square from = MoveHelper::GetFrom(move);
		const Square to = MoveHelper::GetTo(move);
		const MoveHelper::Flag flag = MoveHelper::GetFlag(move);
		const Piece movingP = GetPiece(from);

		
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
		SetPiece(&from, &nullPiece);
		SetPiece(&to, &newP);


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

	Piece GetPiece(const Square index) const {
		Bitboard sm = SquareMask(&index);
		int piece = Piece::None;

		if		((_pawns	& sm) != 0)	piece |= Piece::Pawn;
		else if ((_knights	& sm) != 0)	piece |= Piece::Knight;
		else if ((_bishops	& sm) != 0)	piece |= Piece::Bishop;
		else if ((_rooks	& sm) != 0)	piece |= Piece::Rook;
		else if ((_queens	& sm) != 0)	piece |= Piece::Queen;
		else if ((_kings	& sm) != 0)	piece |= Piece::King;

		if		(IsWhite(&index)) piece |= Piece::White;
		else if (IsBlack(&index)) piece |= Piece::Black;

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



	std::string ToSring(Bitboard highlightSquares) {
		std::stringstream ss;


		ss << "   _______________\n";
		for (int rank = 8; rank >= 1; rank--) {
			ss << rank << "  ";

			for (int file = 8; file >= 1; file--) {
				int idx = Misc::SquareIndex(rank, file);
				auto p = GetPiece(idx);
				char c = PieceChars[p]; 
				if (c == PieceChars[Piece::None] &&
					ValueAtSquare(&highlightSquares, idx)) {
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

