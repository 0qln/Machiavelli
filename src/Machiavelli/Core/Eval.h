#pragma once

#include "Board.h"

namespace Machiavelli {

	class StaticEval {

	public:

		static const Score PieceValues[7];

		/// <summary>
		/// Static position evaluation
		/// </summary>
		/// <param name="board"></param>
		/// <param name="color"></param>
		/// <returns></returns>
		static Score Evaluate(Board* board, Color color);

	};
}