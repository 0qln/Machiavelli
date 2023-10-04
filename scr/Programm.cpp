#include <iostream>
#include <string>

#include "Misc.hpp"
#include "Move.hpp"
#include "Board.hpp"


int main()
{
	Board b = Board::Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	Move e4 = MoveHelper::Create("e2e4");
	Move e5 = MoveHelper::Create("e7e5");

	b.MakeMove(&e4);
	b.MakeMove(&e5);

	std::cout << b.ToSring();
}