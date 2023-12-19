#pragma once

#include <string>
#include <map>

#include "Misc.h"
#include "MoveGen.h"



namespace Machiavelli
{
	class MoveGen;


	struct BoardState {
		BoardState* previous;
		Piece capturedPiece;
		Square capturedSquare;
		Move move;
		int ply;
		bool* kingCastle;
		bool* queenCastle;

		BoardState() {
			queenCastle = new bool[2];
			kingCastle = new bool[2];
			ply = 0;
			move = 0;
			capturedSquare = 0;
			capturedPiece = Piece(0);
			previous = __nullptr;
		}

		~BoardState() {
			delete[] kingCastle;
			delete[] queenCastle;
		}
	};


	class Board
	{
	private:

		int _ply = 0;

		int _move = 0;

		Color _turn = Color::White;

		bool* _kingCastle; 
		bool* _queenCastle;

		Bitboard* _pieceTypes;
		Bitboard* _pieceColors;
		Bitboard _enPassantBitboard;
		Square _enPassantSquare;

		BoardState _currentState;

		void ResetEnpassant();

		void SetEnpassant(Square idx);

		void PopState();

		void SetState(BoardState* newState);



	public:

		void Print();
		void PrintPieceTypeBitboard(PieceType pt);
		void PrintPieceTypeBitboard(Piece p);
		void PrintColorBitboard(Color c);
		void PrintColorBitboard(Piece p);
		void PrintPieceBitboard(PieceType pt, Color c);
		void PrintPieceBitboard(Piece p);
		void PrintCastlingRights();

		std::map<Piece, char> PieceChars = {
			std::make_pair(Piece::WhiteNULL,	'.'),
			std::make_pair(Piece::BlackNULL,	'.'),

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


		Bitboard GetAttacks(Color color);


		bool GetCastlingRights(Color color, bool kingSide);


		Bitboard GetColorBitboard(Color color);


		Bitboard GetPieceBitboard(PieceType type);


		Bitboard GetEnPassantBitboard();


		Square GetEnPassantSquare();


		void SetTurn(Color turn);


		Color GetTurn();


		bool IsInCheck();
		bool IsInCheck(Color color);


		Bitboard GetCheckBlockades();
		Bitboard GetCheckBlockades(Color color);


		Board(std::string fen);

		void SetFromFEN(std::string fen);

		Board();

		~Board();

		void ChangeTurn();


		void MakeMove(const Move* move, bool changeTurn = true);


		void UndoMove(const Move* move, bool changeTurn = true);


		void Clear();


		Piece GetPiece(const Square index);


		void SetPiece(const Square index, const Piece piece);


		/// <summary>
		/// Convert the board into an ascii format
		/// </summary>
		/// <param name="highlightSquares"></param>
		/// <returns></returns>
		std::string ToString(Bitboard highlightSquares = 0);
	};

}
