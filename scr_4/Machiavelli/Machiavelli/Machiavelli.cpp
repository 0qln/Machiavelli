
#include <iostream>
#include <string>

#include "../dll/TestClass.h"

#include "Misc.h"
#include "Move.h"
#include "Board.h"
#include "MoveGen.h"
#include "BitHelper.h"
#include "BoardTests.h"

int main()
{
	//Board b = Board::Board("7k/R7/8/8/8/8/8/K7 w - - 0 1");
	//
	//std::cout << b.ToString() << '\n';

	//Move m = MoveHelper::Create("a7a8", MoveHelper::QUIET_MOVE_FLAG);
	//b.MakeMove(&m);

	//

	//std::cout << b.ToString() << "\n";
	//std::cout << "Is in check: " << b.IsInCheck(Color::Black) << "\n";



	//Board b = Board::Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	//auto gen = MoveGen::MoveGen(&b);
	//gen.PerftLegal(4, true);

	Machiavelli::TestClass::Run();

}