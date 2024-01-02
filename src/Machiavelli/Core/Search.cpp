#include "pch.h"
#include "Search.h"
#include "Eval.h"
#include "MoveHelper.h"

#include <algorithm>
#include <iostream>
#include <string>


namespace Machiavelli {


	/// <summary>
	/// Sort the values and keys array in descending order according to the comparison results of the keys.
	/// </summary>
	/// <param name="keys"></param>
	/// <param name="values"></param>
	/// <param name="length"></param>
	void InsertionSort(Score* keys, Move* values, const size_t length) {
		int i, j;
		Score key;
		Move value;

		for (i = 1; i < length; i++) {
			key = keys[i];
			value = values[i];
			j = i - 1;

			while (j >= 0 && keys[j] < key) {
				keys[j + 1] = keys[j];
				values[j + 1] = values[j];
				--j;
			}

			keys[j + 1] = key;
			values[j + 1] = value;
		}
	}

	/// <summary>
	/// Sort the root moves according to their score in descending order.
	/// Avarage number of moves is 20~25, so insertion sort should be sufficient.
	/// </summary>
	void Search::SortRoot()
	{
		size_t i, j;

		for (i = 1; i < _rootNodes.size(); i++) {
			RootInfo key = _rootNodes[i];
			j = i - 1;

			while (j >= 0 && _rootNodes[j].score < key.score) {
				_rootNodes[j + 1] = _rootNodes[j];
				--j;
			}

			_rootNodes[j + 1] = key;
		}
	}

	Search::~Search()
	{
	}

	Search::Search(Board* board)
	{
		_board = board;
		auto moves = MoveGen::MoveGen(_board).GenerateLegalMoves();
		_maxDepth = DepthTable::NONE;
		std::transform(moves.begin(), moves.end(), std::back_inserter(_rootNodes),
			[](auto move) { return RootInfo(move); });
		for (Depth depth = 0; depth < DepthTable::MAX; depth++) 
			_searchCache[depth].Initialize(depth);
	}



	void Search::Start(Depth final_depth)
	{
		final_depth = min(final_depth, DepthTable::MAX);

		for (_maxDepth = 1; _maxDepth <= final_depth; _maxDepth++) {

			// Search root moves recursivly.
			Score alpha = -ScoreTable::Infinity, beta = ScoreTable::Infinity;
			Negamax<NodeType::ROOT>(_maxDepth, alpha, beta, &_searchCache[1]);

			// Sort moves according to their evaluations.
			// Good moves come first, so they are searched first.
			SortRoot();

			// UCI info response
			std::cout << "info"
				<< " depth " << _maxDepth
				<< " bestmove " << MoveHelper::ToString(_rootNodes[0].move)
				<< '\n'
				;

		}
	}

	std::string Search::GetPV(Move* line) {
		std::string pv;
		for (Depth depth = 0; depth < _maxDepth; depth++) {
			pv.append(MoveHelper::ToString(line[depth]).append(" "));
		}

		return pv;
	}

	std::string Search::GetPV() {
		std::string pv;
		for (Depth depth = 1; depth <= _maxDepth; depth++) {
			pv.append(MoveHelper::ToString(_searchCache[depth].pv).append(" "));
		}

		return pv;
	}

	template <NodeType TNode>
	Score Search::Negamax(Depth depth, Score alpha, Score beta, SearchInfo* si)
	{
		const bool pvNode = TNode == NodeType::PV;
		const bool rootNode = TNode == NodeType::ROOT;

		// TODO: 
		// Check if game is over and return appropriate score.

		// If not: 
		if (depth <= 0) {

			// TODO: quiescent search

			// Return static evaluation of the board.
			return StaticEval::Evaluate(_board, _board->GetTurn());
		}

		// Variables for the recursive search
		auto moves = MoveGen::MoveGen(_board).GenerateLegalMoves();
		if (moves.size() == 0) return 0; // TODO: Return game over evaluation (checkmate|draw) 
		Score bestScore = alpha;
		size_t bestMoveIdx = 0, moveCounter = 0;


		// TODO: 
		// Sort moves according to various heuristics. PV lines come at 
		// the front, so they will be searched first and with greater accuracy.


		// Iterate legal moves
		for (auto move : moves) {
			 
			// Make move
			_board->MakeMove(&move);

			Score newScore;
			if (pvNode || 
				(rootNode && moveCounter < MULTI_PV)) {
				newScore = -Negamax<PV>(depth - 1, -beta, -alpha, si + 1);
			}
			else {
				newScore = -Negamax<NORMAL>(depth - 1, -beta, -alpha, si + 1);
			}

			// Update root
			if (rootNode) {
				_rootNodes[moveCounter].score = newScore;
			}
			
			// Undo move
			_board->UndoMove(&move);

			// Remember best move
			if (newScore > bestScore) {
				bestScore = newScore;

				if (newScore > alpha) {
					bestMoveIdx = moveCounter;

					if (newScore >= beta) {
						// Fail high
						break;
					}
					else {
						alpha = newScore;
					}
				}
			}

			++moveCounter;
		}

		// Return the evaluation.
		return bestScore;
	}

}