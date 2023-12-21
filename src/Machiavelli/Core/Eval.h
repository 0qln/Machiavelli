#pragma once

#include "Board.h"

namespace Machiavelli {

	class StaticEval {

	public:

		static const Score PieceValues[7];

		// [PieceType] [Eg/Mg] [Square]
		static const Score PieceSquareTables[7][2][64];

		/// <summary>
		/// Static position evaluation
		/// </summary>
		/// <param name="board"></param>
		/// <param name="color"></param>
		/// <returns></returns>
		static Score Evaluate(Board* board, Color color);

	};
}