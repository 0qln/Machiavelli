#pragma once

#include "Board.h"

namespace Machiavelli {

	class StaticEval {

	public:

		static const Score PiecePhaseTotal;
		static const Score PiecePhaseValues[7];


		static const Score PieceValues[7];

		// [PieceType] [Eg/Mg] [Square]
		static const Score PieceSquareTables[7][2][64];

		/// <summary>
		/// Static position evaluation relative to `color` parameter
		/// </summary>
		/// <param name="board"></param>
		/// <param name="color"></param>
		/// <returns></returns>
		static Score Evaluate(Board* board, Color color);

		/// <summary>
		/// Static position evaluation relative to white
		/// </summary>
		/// <param name="board"></param>
		/// <returns></returns>
		static Score Evaluate(Board* board);

		/// <summary>
		/// Interpolate between EG score and MG score depending on the current phase of the game.
		/// </summary>
		/// <param name="board"></param>
		/// <param name="mg"></param>
		/// <param name="eg"></param>
		/// <returns></returns>
		static Score PhaseInterpolation(Board* board, Score mg, Score eg);

		/// <summary>
		/// A number indicating the phase (MG|EG) of the board
		/// </summary>
		/// <param name="board"></param>
		/// <returns></returns>
		static int Phase(Board* board);
	};
}