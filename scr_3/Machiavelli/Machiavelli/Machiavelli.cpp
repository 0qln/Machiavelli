
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

	auto pos = "e4"; 
	Square square = Misc::ToSquareIndex(pos); 
	b.SetPiece(square, BlackKnight);
	b.SetPiece(Misc::ToSquareIndex("e2"), BlackKnight);

	Move movelist[50];
	generator.GeneratePseudoLegalMoves(movelist, Black);


	for (auto move : movelist) {
		std::cout << b.ToSring((1ULL << MoveHelper::GetTo(&move)) | (1ULL << MoveHelper::GetFrom(&move))) << "\n";
	}

	return EXIT_SUCCESS;
}