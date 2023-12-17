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

	auto b2 = Machiavelli::Board::Board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
	auto gen2 = Machiavelli::MoveGen::MoveGen(&b2);

	std::cout << b2.ToString() << '\n';
	
	//std::cout << b2.GetTurn() << '\n';

	//std::cout << b2.GetCastlingRights(Machiavelli::Color::Black, true) << '\n';

	//gen2.Perft(2, true);
	//b2.IsInCheck();
	//auto e8g8 = Machiavelli::MoveHelper::Create("e8g8", Machiavelli::MoveHelper::Flags::KING_CASTLE_FLAG);
	//b2.MakeMove(&e8g8, false);
	//std::cout << b2.IsInCheck() << '\n';

	//std::cout << b2.GetCastlingRights(Machiavelli::Color::Black, true) << '\n';

	gen2.Perft(2, true);

	//std::cout << b2.ToString() << '\n';
}