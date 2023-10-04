#include <iostream>
#include <string>

#include "Misc.hpp"
#include "Move.hpp"
#include "Board.hpp"


int main()
{
	Board b = Board::Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");


	Move moves[8]; 
	moves[0] = MoveHelper::Create("e2e4", MoveHelper::Flags::DOUBLE_PAWN_PUSH_FLAG);
	moves[1] = MoveHelper::Create("e7e5", MoveHelper::Flags::DOUBLE_PAWN_PUSH_FLAG);

	moves[2] = MoveHelper::Create("g1f3", MoveHelper::Flags::QUIET_MOVE_FLAG);
	moves[3] = MoveHelper::Create("b8c6", MoveHelper::Flags::QUIET_MOVE_FLAG);

	moves[4] = MoveHelper::Create("f1b5", MoveHelper::Flags::QUIET_MOVE_FLAG);
	moves[5] = MoveHelper::Create("g8f6", MoveHelper::Flags::QUIET_MOVE_FLAG);

	moves[6] = MoveHelper::Create("e1g1", MoveHelper::Flags::KING_CASTLE_FLAG);
	moves[7] = MoveHelper::Create("d7d6", MoveHelper::Flags::QUIET_MOVE_FLAG);


	for(Move move : moves)
		b.MakeMove(&move);


	std::cout << b.ToSring();
}