#include <iostream>

#include "../Core/Board.h"
#include "../Core/MoveGen.h"
#include "../Core/MoveHelper.h"
#include "../Core/Misc.h"

/// https://gist.github.com/DOBRO/2592c6dad754ba67e6dcaec8c90165bf

int main(int argc, char** argv)
{
	if (argc) {
		// Handle UCI input	
	}

	auto b2 = Machiavelli::Board::Board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
	auto gen2 = Machiavelli::MoveGen::MoveGen(&b2);
	auto e5g4 = Machiavelli::MoveHelper::Create("e5g4", Machiavelli::MoveHelper::QUIET_MOVE_FLAG);
	auto h3g2 = Machiavelli::MoveHelper::Create("h3g2", Machiavelli::MoveHelper::CAPTURE_FLAG);
	auto e5f7 = Machiavelli::MoveHelper::Create("e5f7", Machiavelli::MoveHelper::CAPTURE_FLAG);
	auto a1b1 = Machiavelli::MoveHelper::Create("a1b1", Machiavelli::MoveHelper::QUIET_MOVE_FLAG);


	//gen2.Perft(2, true);

	//b2.MakeMove(&a1b1);
	//b2.UndoMove(&a1b1);

	std::cout << b2.ToString() << '\n';
	b2.PrintCastlingRights();
	
	//std::cout << b2.ToString(b2.GetAttacks(Machiavelli::Color::White)) << '\n';

	gen2.Perft(2, true);

	std::cout << b2.ToString() << '\n';
	b2.PrintCastlingRights();

	//std::cout << '\n';

	//b2.MakeMove(&e5g4, true);
	//gen2.Perft(1, true);
	//b2.UndoMove(&e5g4, true);

	//std::cout << b2.ToString() << '\n';
}