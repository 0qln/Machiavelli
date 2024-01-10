#pragma once

#include "Board.h"

#include <vector>


namespace Machiavelli {

	enum NodeType {
		NORMAL,
		PV,
		ROOT
	};

	struct SearchCancelationToken {
		bool ShouldStop = false;
	};

	class Search {

	private:

		struct SearchInfo {
			Move pv = 0;
			Move* pvLine;
			Depth depth, pvDepth;

// We need a default constructor.
#pragma warning (disable : 26495)
			SearchInfo() 
			{ }
#pragma warning (enable : 26495)

			SearchInfo(Depth d) : depth(d), pvDepth(DepthTable::MAX - depth) {
				// Initialize pv lines
				pvLine = new Move[pvDepth];
				for (int i = 0; i < pvDepth; i++)
					pvLine[i] = 0; // Null Move
			}

			SearchInfo* Initialize(Depth d) {
				depth = d;
				pvDepth = DepthTable::MAX - depth;
				pvLine = new Move[pvDepth];

				// Initialize pv lines
				for (int i = 0; i < pvDepth; i++)
					pvLine[i] = 0; // Null Move

				return this;
			}

			~SearchInfo() {
				delete[] pvLine;
			}
		};

		struct RootInfo {
			Score score;
			Move move;

			RootInfo(Move move) : score(-ScoreTable::Infinity), move(move) 
			{ }
		};

		Board* _board;

		// Indexed with depth
		SearchInfo _searchCache[DepthTable::MAX];

		const static int MULTI_PV = 1;

		Depth _maxDepth;

		size_t _nodesSearched;

		std::vector<RootInfo> _rootNodes;

		void SortRoot();


	public:

		SearchCancelationToken CancelationToken;

		
		~Search();


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
		template <NodeType TNode>
		Score Negamax(Depth depth, Score alpha, Score beta, SearchInfo* si);

#pragma region DepricatedPVExtraction
		std::string GetPV();

		std::string GetPV(Move* line);
#pragma endregion

	};
}