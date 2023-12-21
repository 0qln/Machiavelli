#pragma once

#include "Board.h"

#include <vector>


namespace Machiavelli {

	enum SearchMode {
		NORMAL,
		PV,
	};

	class Search {

	private:

		Board* _board;

		// Indexed with depth
		Move _pv[DepthTable::MAX];

		const int MULTI_PV = 1;


	public:

		/// <summary>
		/// Initiate a search.
		/// </summary>
		/// <param name="board">: The Board to be searched.</param>
		Search(Board* board);


		/// <summary>
		/// Main iterative deepening loop.
		/// </summary>
		void Start(const Depth max_depth = DepthTable::MAX);


		/// <summary>
		/// 
		/// </summary>
		/// <param name="depth"></param>
		/// <returns></returns>
		Score NegaMax(Depth depth, Score alpha, Score beta, SearchMode sm);
	};
}