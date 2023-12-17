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

	auto b2 = Machiavelli::Board::Board("r3k2r/p1ppqNb1/bn2pnp1/3P4/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQkq - 0 1");
	auto gen2 = Machiavelli::MoveGen::MoveGen(&b2);
	auto e5g4 = Machiavelli::MoveHelper::Create("e5g4", Machiavelli::MoveHelper::QUIET_MOVE_FLAG);
	auto h3g2 = Machiavelli::MoveHelper::Create("h3g2", Machiavelli::MoveHelper::CAPTURE_FLAG);
	auto e5f7 = Machiavelli::MoveHelper::Create("e5f7", Machiavelli::MoveHelper::CAPTURE_FLAG);


	//b2.MakeMove(&e5f7);

	std::cout << b2.ToString() << '\n';
	std::cout << b2.ToString(b2.GetAttacks(Machiavelli::Color::White)) << '\n';

	gen2.Perft(1, true);

	//std::cout << '\n';

	//b2.MakeMove(&e5g4, true);
	//gen2.Perft(1, true);
	//b2.UndoMove(&e5g4, true);

	//std::cout << b2.ToString() << '\n';
}