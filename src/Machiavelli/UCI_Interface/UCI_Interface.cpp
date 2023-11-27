#include <iostream>

#include "../Core/Board.h"
#include "../Core/MoveGen.h"

int main()
{

	Machiavelli::Board b = Machiavelli::Board::Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	auto gen = Machiavelli::MoveGen::MoveGen(&b);
	gen.PerftLegal(4, true);
}