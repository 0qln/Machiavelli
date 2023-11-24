
#include <iostream>
#include <string>

#include "Misc.h"
#include "Move.h"
#include "Board.h"
#include "MoveGen.h"
#include "BitHelper.h"


unsigned long long  makeMoves(Board* pB, int maxDepth, const int initialDepth, bool pv) {
	unsigned long long moveCount = 0;

	if (maxDepth == 0)
		return 1;

	auto gen = MoveGen::MoveGen(pB);
	auto movelist = gen.GeneratePseudoLegalMoves(pB->GetTurn());

	for (int i = 0; i < movelist.size(); i++) {
		pB->MakeMove(&movelist[i]);
		
		bool movePV = false;
		int count = makeMoves(pB, maxDepth - 1, initialDepth, movePV);
		if (pv) {
			std::cout << pB->ToSring() << "\n";
			std::cout << MoveHelper::ToString(movelist[i]) << ": " << count << "\n";
		}
		else {
			assert(true);
		}
		moveCount += count;

		pB->UndoMove(&movelist[i]);
	}

	return moveCount;
}

int main() {
	Board b = Board::Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	MoveGen generator = MoveGen::MoveGen(&b); 
	//std::cout << b.ToSring();

	std::cout << makeMoves(&b, 4, 0, true);


	return EXIT_SUCCESS;
}
 