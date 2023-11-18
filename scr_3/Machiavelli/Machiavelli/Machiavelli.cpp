
#include <iostream>
#include <string>

#include "Misc.hpp"
#include "Move.hpp"
#include "Board.hpp"
#include "MoveGen.hpp"


int main()
{
	Board b = Board::Board("8/8/8/8/8/8/8/8 w - - 0 1");

	auto bishopPos = "d4"; 

	b.SetPiece(Misc::ToSquareIndex(bishopPos), Piece::WhiteBishop);
	//b.SetPiece(Misc::ToSquareIndex("d7"), Piece::WhitePawn); // left
	//b.SetPiece(Misc::ToSquareIndex("d1"), Piece::WhitePawn); // left
	//b.SetPiece(Misc::ToSquareIndex("d6"), Piece::BlackPawn); // down 
	//b.SetPiece(Misc::ToSquareIndex("e7"), Piece::BlackPawn); // down 
	//b.SetPiece(Misc::ToSquareIndex("g4"), Piece::BlackPawn); // right

	MoveGen generator = MoveGen::MoveGen(&b); 
	Bitboard moves = generator.GenerateBishopMoves(Misc::ToSquareIndex(bishopPos)); 

	//std::cout << b.ToSring(moves);


	std::cout << b.ToSring(generator.DiagMask2[0]);
	std::cout << b.ToSring(generator.DiagMask2[1]);
	std::cout << b.ToSring(generator.DiagMask2[2]);
	std::cout << b.ToSring(generator.DiagMask2[3]);
	std::cout << b.ToSring(generator.DiagMask2[4]);
	std::cout << b.ToSring(generator.DiagMask2[5]);
	std::cout << b.ToSring(generator.DiagMask2[6]);
	std::cout << b.ToSring(generator.DiagMask2[7]);
	std::cout << b.ToSring(generator.DiagMask2[8]);
	std::cout << b.ToSring(generator.DiagMask2[9]);
	std::cout << b.ToSring(generator.DiagMask2[10]);
	std::cout << b.ToSring(generator.DiagMask2[11]);
	std::cout << b.ToSring(generator.DiagMask2[12]);
	std::cout << b.ToSring(generator.DiagMask2[13]);
	std::cout << b.ToSring(generator.DiagMask2[14]);

	return EXIT_SUCCESS;
}