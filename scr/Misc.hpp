#pragma once

using Bitboard = unsigned __int64;


enum Depth {
	NONE = 0,
	MAX = 255,
};


using Score = __int32;


enum Piece : int {

	None =			0b00000,

	TypeMask =		0b11100,
	ColorMask =		0b00011,
	
	White =			0b00010,
	Black =			0b00001,

	Pawn =			0b00100,
	Knight =		0b01000,
	Bishop =		0b01100,
	Rook =			0b10000,
	Queen =			0b10100,
	King =			0b11000,

	BlackPawn =		0b00101,
	BlackKnight =	0b01001,
	BlackBishop =	0b01101,
	BlackRook =		0b10001,
	BlackQueen =	0b10101,
	BlackKing =		0b11001,

	WhitePawn =		0b00110,
	WhiteKnight =	0b01010,
	WhiteBishop =	0b01110,
	WhiteRook =		0b10010,
	WhiteQueen =	0b10110,
	WhiteKing =		0b11010,

};


using Move = unsigned __int16;


using Square = unsigned __int8;