#include <iostream>
#include <string>

#include "Misc.hpp"
#include "Move.hpp"
#include "Board.hpp"
#include "MoveGen.hpp"


int main()
{
	Board b = Board::Board("8/8/8/8/8/8/8/8 w - - 0 1");

	b.SetPiece(Misc::ToSquareIndex("e4"), Piece::WhiteRook);

	//MoveGen generator = MoveGen::MoveGen(&b);
	//Bitboard rookMoves = generator.GenerateRookMoves(Misc::ToSquareIndex("e4"));

	std::cout << b.ToSring(0);

	return EXIT_SUCCESS;
}