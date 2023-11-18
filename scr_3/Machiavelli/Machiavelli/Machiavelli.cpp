
#include <iostream>
#include <string>

#include "Misc.hpp"
#include "Move.hpp"
#include "Board.hpp"
#include "MoveGen.hpp"


int main()
{
	Board b = Board::Board("8/8/8/8/8/8/8/8 w - - 0 1");

	auto rookPos = "d4";

	b.SetPiece(Misc::ToSquareIndex(rookPos), Piece::WhiteRook);
	b.SetPiece(Misc::ToSquareIndex("d7"), Piece::WhitePawn); // left
	b.SetPiece(Misc::ToSquareIndex("d1"), Piece::WhitePawn); // left
	b.SetPiece(Misc::ToSquareIndex("d6"), Piece::BlackPawn); // down 
	b.SetPiece(Misc::ToSquareIndex("e7"), Piece::BlackPawn); // down 
	b.SetPiece(Misc::ToSquareIndex("g4"), Piece::BlackPawn); // right

	MoveGen generator = MoveGen::MoveGen(&b); 
	Bitboard rookMoves = generator.GenerateWhiteRookMoves(Misc::ToSquareIndex(rookPos)); 

	std::cout << b.ToSring(rookMoves);

	return EXIT_SUCCESS;
}