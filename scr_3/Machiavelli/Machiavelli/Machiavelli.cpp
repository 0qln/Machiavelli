
#include <iostream>
#include <string>

#include "Misc.hpp"
#include "Move.hpp"
#include "Board.hpp"
#include "MoveGen.hpp"


int main()
{
	Board b = Board::Board("8/8/8/8/8/8/8/8 w - - 0 1");
	MoveGen generator = MoveGen::MoveGen(&b); 

	//for (File x = FileTable::A; x <= FileTable::H; x++) {
	//	for (Rank y = 0; y <= 7; y++) {
	//		b.Clear();
	//		Square square = Misc::SquareIndex_0IDX(y, x);
	//		b.SetPiece(square, Piece::WhiteBishop); 
	//		Bitboard moves = generator.GenerateBishopMoves(square); 
	//		std::cout << b.ToSring(moves); 
	//	}
	//}

	auto bishopPos = "d4"; 
	Square bishopSquare = Misc::ToSquareIndex(bishopPos); 

	b.SetPiece(bishopSquare, Piece::WhiteBishop);
	b.SetPiece(Misc::ToSquareIndex("c3"), Piece::WhitePawn);
	b.SetPiece(Misc::ToSquareIndex("a1"), Piece::WhitePawn);

	Bitboard moves = generator.GenerateWhiteBishopMoves(bishopSquare);
	std::cout << b.ToSring(moves);

	return EXIT_SUCCESS;
}