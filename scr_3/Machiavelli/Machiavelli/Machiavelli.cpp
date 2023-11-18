
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

	auto pos = "d7"; 
	Square square = Misc::ToSquareIndex(pos); 
	b.SetPiece(square, BlackPawn);


	Move move; 
	b.SetPiece(Misc::ToSquareIndex("e2"), WhitePawn);
	b.SetTurn(Color::White); 
	move = MoveHelper::Create("e2e4", MoveHelper::DOUBLE_PAWN_PUSH_FLAG);
	b.MakeMove(&move);

	b.SetPiece(Misc::ToSquareIndex("c3"), WhitePawn);


	Bitboard moves = generator.GeneratePawnMoves(square);
	std::cout << b.ToSring(moves);

	return EXIT_SUCCESS;
}