#pragma once

#include <string>


using Bitboard = unsigned __int64;

enum Color {
	White, Black
};

enum Depth {
	NONE = 0,
	MAX = 255,
};

enum CompassRose : int {
	Zero,

	Nort = +8,
	Sout = -8,
	West = -1,
	East = +1,

	NoWe = Nort + West,
	NoEa = Nort + East,
	SoWe = Sout + West,
	SoEa = Sout + East,
};

using Score = __int32;

enum PieceType : int {
	PT_NULL =		0,
	Pawn =			1,
	Knight =		2,
	Bishop =		3,
	Rook =			4,
	Queen =			5,
	King =			6,
};

using Square = int;
enum SquareTable : int {
	A1, B1, C1, D1, E1, F1, G1, H1,
	A2, B2, C2, D2, E2, F2, G2, H2,
	A3, B3, C3, D3, E3, F3, G3, H3,
	A4, B4, C4, D4, E4, F4, G4, H4,
	A5, B5, C5, D5, E5, F5, G5, H5,
	A6, B6, C6, D6, E6, F6, G6, H6,
	A7, B7, C7, D7, E7, F7, G7, H7,
	A8, B8, C8, D8, E8, F8, G8, H8,
};
using File = int;
enum FileTable : File { A, B, C, D, E, F, G, H };
using Rank = int;
enum RankTable : Rank { };

enum Piece : int {
	WhiteNULL = 0,
	BlackNULL = 1,

	ColorMask = 1, 
	TypeMask = ~ColorMask,

	BlackPawn =		Black | (Pawn<<1)	,
	BlackKnight =	Black | (Knight<<1)	,
	BlackBishop =	Black | (Bishop<<1)	,
	BlackRook =		Black | (Rook<<1)	,
	BlackQueen =	Black | (Queen<<1)	,
	BlackKing =		Black | (King<<1)	,
	
	WhitePawn =		White | (Pawn<<1)	,
	WhiteKnight =	White | (Knight<<1)	,
	WhiteBishop =	White | (Bishop<<1)	,
	WhiteRook =		White | (Rook<<1)	,
	WhiteQueen =	White | (Queen<<1)	,
	WhiteKing =		White | (King<<1)	,
};


using Move = unsigned __int16;


class Misc {
public:

	/// <summary>
	/// receives for example 'e3'
	/// </summary>
	/// <param name="square"></param>
	/// <returns></returns>
	static inline Square ToSquareIndex(const std::string* square) {
		File file =	square->at(0) - 'a';
		Rank rank = square->at(1) - '1';
		return SquareIndex_0IDX(rank, file);
	}
	static inline std::string FromSquareIndex(const Square square) {
		char file = (square % 8) + 'a';
		char rank = (square / 8) + '1';
		return std::string({ file, rank });
	}
	static inline Square ToSquareIndex(const std::string square) {
		return ToSquareIndex(&square);
	}


	/// <summary>
	/// 
	/// </summary>
	/// <param name="row">1-indexed</param>
	/// <param name="file">1-indexed</param>
	/// <returns>0-indexed</returns>
	static inline Square SquareIndex_1IDX(const Rank rank, const File file) {
		return file + 8 * (rank - 1) - 1;
	}


	/// <summary>
	/// 
	/// </summary>
	/// <param name="row">0-indexed</param>
	/// <param name="file">0-indexed</param>
	/// <returns>0-indexed</returns>
	static inline Square SquareIndex_0IDX(const Rank rank, const File file) {
		return file + 8 * rank;
	}
};