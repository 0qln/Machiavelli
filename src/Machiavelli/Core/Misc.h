#pragma once

#include <string>


namespace Machiavelli
{
	/// <summary>
	/// <para>64-bit Bitboard</para>
	/// https://www.chessprogramming.org/Bitboards
	/// </summary>
	using Bitboard = unsigned __int64;

	/// <summary>
	/// <para>Side-to-move and Piece color</para>
	/// <para>White = 0</para>
	/// <para>Black = 1</para>
	/// </summary>
	enum Color : bool {
		White, Black
	};

	/// <summary>
	/// Search depth
	/// </summary>
	using Depth = int;
	enum DepthTabe : int {
		NONE = 0,
		MAX = 255,
	};

	/// <summary>
	/// Compass rose the work with directions
	/// </summary>
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

	/// <summary>
	/// Evaluation score for game tree branches
	/// </summary>
	using Score = __int32;
	enum ScoreTable : Score {
		Infinity = 30000,

		PawnScore = 100,
		KnightScore = 300,
		BishopScore = 350,
		RookScore = 500,
		QueenScore = 850,
		KingScore = 9999
	};

	/// <summary>
	/// Piece type order; compatible with <see cref="Piece"/> and useful for indexing
	/// </summary>
	enum PieceType : int {
		PT_NULL = 0,
		Pawn = 1,
		Knight = 2,
		Bishop = 3,
		Rook = 4,
		Queen = 5,
		King = 6,
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

	/// <summary>
	/// Piece ordering and masks, compatible with <see cref="PieceType"/> and <see cref="Color"/>
	/// </summary>
	enum Piece : int {
		WhiteNULL = 0,
		BlackNULL = 1,

		ColorMask = 1,
		TypeMask = ~ColorMask,

		BlackPawn	= Black | (Pawn << 1),
		BlackKnight = Black | (Knight << 1),
		BlackBishop = Black | (Bishop << 1),
		BlackRook	= Black | (Rook << 1),
		BlackQueen	= Black | (Queen << 1),
		BlackKing	= Black | (King << 1),

		WhitePawn	= White | (Pawn << 1),
		WhiteKnight = White | (Knight << 1),
		WhiteBishop = White | (Bishop << 1),
		WhiteRook	= White | (Rook << 1),
		WhiteQueen	= White | (Queen << 1),
		WhiteKing	= White | (King << 1),
	};

	/// <summary>
	/// <para>Representation of a move</para>
	/// <para>First 4 bits: Flags for additional info</para>
	/// <para>Next 6 bits: To-Square index</para>
	/// <para>Next 6 bits: From-Square index</para>
	/// </summary>
	using Move = unsigned __int16;

	/// <summary>
	/// Utils
	/// </summary>
	class Misc
	{
	public:
		/// <summary>
		/// Create a SquareIndex from a string.
		/// </summary>
		/// <param name="square">The square in a (rank-file) notation</param>
		/// <returns>The string as a square</returns>
		static Square ToSquareIndex(const std::string* square);

		/// <summary>
		/// Convert the Square into (file-rank) notiation.
		/// </summary>
		/// <param name="square">The square</param>
		/// <returns>String representing the square</returns>
		static std::string FromSquareIndex(const Square square);

		/// <summary>
		/// Convert the rank and file coordinates to a Square index.
		/// </summary>
		/// <param name="rank">One-Indexed</param>
		/// <param name="file">One-Indexed</param>
		/// <returns>Zero-indexed square index</returns>
		static Square SquareIndex1(const Rank rank, const File file);

		/// <summary>
		/// Convert the rank and file coordinates to a Square index.
		/// </summary>
		/// <param name="rank">Zero-Indexed</param>
		/// <param name="file">Zero-Indexed</param>
		/// <returns>Zero-indexed square index</returns>
		static Square SquareIndex0(const Rank rank, const File file);
	};


}