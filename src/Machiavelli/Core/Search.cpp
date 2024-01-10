#include "pch.h"
#include "Search.h"
#include "Eval.h"
#include "MoveHelper.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <chrono>

#define BENCHMARK

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
		// Using size_t does cause an overflow, causing inner loop condition to fail
		// and eventually crash the program. Altough this only occurs in Debug mode,
		// we use signed integers here.
		int i, j;

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
		auto moves = MoveGen::MoveGen(_board).GenerateLegalMovesConfident();
		_maxDepth = DepthTable::NONE;
		// Fill root nodes with root info
		std::transform(moves.begin(), moves.end(), std::back_inserter(_rootNodes),
			[](auto move) { return RootInfo(move); }
		);
		// Fill search cache
		for (Depth depth = 0; depth < DepthTable::MAX; depth++) {
			_searchCache[depth].Initialize(depth);
		}
	}

	void Search::Start(Depth final_depth)
	{
#ifdef BENCHMARK
		using std::chrono::high_resolution_clock;
		using std::chrono::duration;
		using std::chrono::milliseconds;
		auto begin = high_resolution_clock::now();
		_nodesSearched = 0;
#endif
		final_depth = min(final_depth, DepthTable::MAX);

		for (_maxDepth = 1; _maxDepth <= final_depth; _maxDepth++) {

			// Search root moves recursivly.
			Score alpha = -ScoreTable::Infinity, beta = ScoreTable::Infinity;
			Negamax<NodeType::ROOT>(_maxDepth, alpha, beta, &_searchCache[1]);

			// Sort moves according to their evaluations.
			// Good moves come first, so they are searched first.
			SortRoot();

			// UCI info response
			duration<double, std::milli> ms = high_resolution_clock::now() - begin;
			std::cout << "info"
				<< " depth " << _maxDepth
				<< " bestmove " << MoveHelper::ToString(_rootNodes[0].move)
#ifdef BENCHMARK
				<< " nodes " << _nodesSearched
				<< " time " << ms.count()
				<< " nps " << (_nodesSearched / ms.count()) * 1000
#endif
				<< '\n'
				;

			// Stop calculation if requested
			if (CancelationToken.ShouldStop) {
				break;
			}
		}
	}

#pragma region DepricatedPVExtraction

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

#pragma endregion

	template <NodeType TNode>
	Score Search::Negamax(Depth depth, Score alpha, Score beta, SearchInfo* si)
	{
#ifdef BENCHMARK
		_nodesSearched++;
#endif

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
		auto moves = MoveGen::MoveGen(_board).GenerateLegalMovesConfident();
		if (moves.size() == 0) return 0; // TODO: Return game over evaluation (checkmate|draw) 
		Score bestScore = alpha;
		size_t bestMoveIdx = 0;


		// TODO: 
		// Sort moves according to various heuristics. PV lines come at 
		// the front, so they will be searched first and with greater accuracy.


		// Iterate legal moves
		for (size_t moveCounter = 0; moveCounter < moves.size(); ++moveCounter) {
			// Stop calculation if requested
			if (CancelationToken.ShouldStop) {
				// Incomplete searches should not be trusted, thus we return a score
				// s.d. this node will not be looked considered. All previously
				// calculated node evaluations will be discarded.
				return ScoreTable::Infinity;
			}

			// Make move
			_board->MakeMove(&moves[moveCounter]);

			Score newScore;
			if (pvNode || (rootNode && moveCounter < MULTI_PV)) {
				newScore = -Negamax<PV>(depth - 1, -beta, -alpha, si + 1);
			}
			else {
				newScore = -Negamax<NORMAL>(depth - 1, -beta, -alpha, si + 1);
			}

			// Update root
			if (rootNode && !CancelationToken.ShouldStop) {
				_rootNodes[moveCounter].score = newScore;
			}

			// Undo move
			_board->UndoMove(&moves[moveCounter]);

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
		}

		// Return the evaluation.
		return bestScore;
	}

}