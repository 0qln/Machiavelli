#pragma once

#include "Board.h"

namespace Machiavelli {

	class Search {

	private:

		Board* _board;


	public:

		/// <summary>
		/// Initiate a search.
		/// </summary>
		/// <param name="board">: The Board to be searched.</param>
		Search(Board* board);


		/// <summary>
		/// Main iterative deepening loop.
		/// </summary>
		void Start();


		/// <summary>
		/// 
		/// </summary>
		/// <param name="depth"></param>
		/// <returns></returns>
		Score NegaMax(Depth depth, Color color);
	};
}