#include <iostream>
#include <string>

#include "Misc.hpp"
#include "Move.hpp"
#include "Board.hpp"


int main()
{
	//Board b = Board::Board("B6R/8/8/8/8/8/8/P6N w - - 0 1");

	Board b = Board::Board("8/8/8/8/8/8/8/8 w - - 0 1");
	//b.SetPiece(SquareTable::H8, Piece::WhitePawn);
	b.SetPiece(Misc::ToSquareIndex("h8"), Piece::WhitePawn);

	//int rank = b.Turn() == Color::White ? Rank(0) : Rank(7);
	//auto kingMove = MoveHelper::Create(rank + FileTable::E, rank + FileTable::C, MoveHelper::QUITE_MOVE_MASK);
	//auto rookMove = MoveHelper::Create(rank + FileTable::A, rank + FileTable::D, MoveHelper::QUITE_MOVE_MASK);
	//b.MakeMove(&kingMove);
	//b.MakeMove(&rookMove);

	//auto m = MoveHelper::Create(0,0, MoveHelper::Flags::QUEEN_CASTLE_FLAG);
	//b.MakeMove(&m);


	std::cout << b.ToSring(0);


	return EXIT_SUCCESS;
}