#include <iostream>
#include <string>

#include "Misc.hpp"
#include "Move.hpp"
#include "Board.hpp"


int main()
{
	Board b = Board::Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

	std::cout << int(Misc::ToSquareIndex("a1")) << " [7]" << "\n";
	std::cout << int(Misc::ToSquareIndex("h1")) << " [0]" << "\n";
	std::cout << int(Misc::ToSquareIndex("a8")) << " [63]" << "\n";
	std::cout << int(Misc::ToSquareIndex("h8")) << " [56]" << "\n";


	Move moves[6]; 
	moves[0] = MoveHelper::Create("e2e4", MoveHelper::Flags::DOUBLE_PAWN_PUSH_FLAG);
	moves[1] = MoveHelper::Create("e7e5", MoveHelper::Flags::DOUBLE_PAWN_PUSH_FLAG);

	moves[2] = MoveHelper::Create("g1f3", MoveHelper::Flags::QUIET_MOVE_FLAG);
	moves[3] = MoveHelper::Create("b8c6", MoveHelper::Flags::QUIET_MOVE_FLAG);

	moves[4] = MoveHelper::Create("f1b5", MoveHelper::Flags::QUIET_MOVE_FLAG);
	moves[5] = MoveHelper::Create("g8f6", MoveHelper::Flags::QUIET_MOVE_FLAG);

	for(Move move : moves)
		b.MakeMove(&move);


	std::cout << b.ToSring();
}