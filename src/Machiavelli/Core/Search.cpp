#include "pch.h"
#include "Search.h"
#include "Eval.h"

#include <math.h>

namespace Machiavelli {

	Search::Search(Board* board)
	{
		_board = board;
	}

	void Search::Start()
	{

		for (Depth depth = 1; depth <= DepthTabe::MAX; depth++) {

		}

	}

	Score Search::NegaMax(Depth depth, Color color)
	{
		if (depth == 0) {
			return StaticEval::Evaluate(_board, color);
		}

		Score score = -ScoreTable::Infinity;

		for (auto move : MoveGen::MoveGen(_board).GenerateLegalMoves())
		{
			score = max(score, -NegaMax(depth - 1, Color(!color)));
		}

		return score;
	}

}