
#include <iostream>
#include <string>

#include "Misc.h"
#include "Move.h"
#include "Board.h"
#include "MoveGen.h"
#include "BitHelper.h"
#include "BoardTests.h"

int main()
{
	Board b = Board::Board("7k/R7/8/8/8/8/8/K7 w - - 0 1");
	
	std::cout << b.ToString() << '\n';

	Move m = MoveHelper::Create("a7a8", MoveHelper::QUIET_MOVE_FLAG);
	b.MakeMove(&m);
}