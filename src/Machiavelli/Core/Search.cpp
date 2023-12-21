#include "pch.h"
#include "Search.h"
#include "Eval.h"
#include "MoveHelper.h"

#include <algorithm>
#include <iostream>
#include <string>


namespace Machiavelli {

	/// <summary>
	/// Sort the values and keys array according to the comparison results os the keys.
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

			while (j >= 0 && keys[j] > key) {
				keys[j + 1] = keys[j];
				values[j + 1] = values[j];
				--j;
			}
			keys[j + 1] = key;
			values[j + 1] = value;
		}
	}


	Search::Search(Board* board)
	{
		_board = board;
		for (int i = 0; i < DepthTable::MAX; i++) _pv[i] = 0;
	}

	void Search::Start(Depth max_depth)
	{
		auto moves = MoveGen::MoveGen(_board).GenerateLegalMoves();
		const int move_count = moves.size();
		Score* scores = new Score[move_count];

		for (Depth depth = 1; depth <= max_depth; depth++) {
			int multi_pv = MULTI_PV;

			// Get Evaluations
			for (int i = 0; i < move_count; i++) {
				_board->MakeMove(&moves[i]);
				scores[i] = -NegaMax(depth, -ScoreTable::Infinity, ScoreTable::Infinity, multi_pv-- > 0 ? PV : NORMAL);
				_board->UndoMove(&moves[i]);
			}

			// Sort moves according to their evaluations
			InsertionSort(scores, moves.data(), move_count);

			std::string pv;
			for (int i = depth; i > 0; i--) {
				pv.append(MoveHelper::ToString(_pv[i]));
				pv.append(" ");
			}

			std::cout << "info"
				<< " depth " << depth
				<< " pv " << pv
				<< '\n';
		}

		delete[] scores;
	}

	Score Search::NegaMax(Depth depth, Score alpha, Score beta, const SearchMode sm)
	{
		// TODO: check if game is over

		if (depth <= 0) {

			// TODO: quiescent search

			return StaticEval::Evaluate(_board, _board->GetTurn());
		}


		auto moves = MoveGen::MoveGen(_board).GenerateLegalMoves();
		
		if (moves.size() == 0) {
			return 0;
		}

		Score score = -ScoreTable::Infinity;

		Move* best_move = &moves[0];

		// TODO: sort moves according to various heuristics

		int multi_pv = MULTI_PV;
		
		for (auto move : moves) {
			_board->MakeMove(&move);
			auto new_score = -NegaMax(depth - 1, -beta, -alpha, multi_pv-- > 0 ? sm : NORMAL);
			_board->UndoMove(&move);
			
			if (new_score > score) {
				best_move = &move;
				score = new_score;
			}

			// Alpha beta pruning
			if (alpha >= beta) {
				break;
			}
		}

		if (sm == PV) {
			_pv[depth] = *best_move;
		}

		return score;
	}

}