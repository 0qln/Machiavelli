#include "UCI_Commands.h"

#include "../Core/Misc.h"
#include "../Core/MoveHelper.h"

#include <iostream>
#include <chrono>


namespace UCI {
	Command UCI::ParseCommand(const std::string& token) {
		const std::map<std::string, Command> map_commands = {
			{ "position", Command::POSITION },
			{ "go", Command::GO },
			{ "quit", Command::QUIT },
			{ "uci", Command::UCI },
			{ "isready", Command::IS_READY },
			{ "ucinewgame", Command::UCI_NEW_GAME },
		};

		if (map_commands.find(token) == map_commands.end()) {
			return Command::NONE;
		}
		return map_commands.at(token);
	}

	bool ExecuteCommand::Position(std::string fen, std::vector<std::string> moves, Machiavelli::Board* board)
	{
		// Set up board
		/* Save options, if the minimal work apprach appears to fail. */
		*board = Machiavelli::Board::FromFEN(fen);  
		/* */
		
		 // only need to set up the fen, we can assume that `board` is cleared, because
		// the gui should always sent Commands::UCI_NEW_GAME to notify a reset, before 
		// requesting a position to be set up.
		/* board->SetUpFEN(fen); */

		// Make moves
		for (auto move : moves) {
			auto plegals = Machiavelli::MoveGen(board).GeneratePseudoLegalMoves();

			auto it = std::find_if(plegals.begin(), plegals.end(), [&move](const Machiavelli::Move& m) {
				return Machiavelli::MoveHelper::ToString(m) == move;
			});

			if (it != plegals.end()) {
				board->MakeMove(&(*it));
			}
			else {
				return false;
			}
		}

		return true;
	}

	std::optional<Machiavelli::SearchCancelationToken> ExecuteCommand::Go(std::string task, std::vector<std::string> tokens, Machiavelli::Board* board)
	{
		auto contains = [tokens](std::string value) { return std::find(tokens.begin(), tokens.end(), value) != tokens.end(); };

		Machiavelli::Depth depth = Machiavelli::DepthTable::NONE;

		if (task == "depth") {
			depth = std::stoi(tokens[0]);
		}
		else if (task == "infinite") {
			depth = Machiavelli::DepthTable::MAX;
		}

		if (task == "perft") {
			using std::chrono::high_resolution_clock;
			using std::chrono::duration;
			using std::chrono::milliseconds;

			const bool uci = contains("uci");

			auto t1 = high_resolution_clock::now();
			auto moveCount = Machiavelli::MoveGen::MoveGen(board).Perft(std::stoi(tokens[0]), true, uci);
			auto t2 = high_resolution_clock::now();
			duration<double, std::milli> ms = t2 - t1;

			// Give info using the `info` response
			std::cout << "info" 
				<< " nodes " << moveCount
				<< " time " << ms.count()
				<< " nps " << (moveCount / ms.count()) * 1000
				<< '\n'; 

			// No search started -> Return no cancelation token.
			return std::optional<Machiavelli::SearchCancelationToken>();
		}

		// Initialize and start the search.
		auto search = Machiavelli::Search(board);
		search.Start(depth);
		
		// Return the cancelation token.
		return std::optional<Machiavelli::SearchCancelationToken>(search.CancelationToken);
	}
	
	void ExecuteCommand::Quit()
	{
		exit(EXIT_SUCCESS);
	}

	void ExecuteCommand::Uci()
	{
		std::cout << "id name " << "Machiavelli" << '\n';
		std::cout << "id author " << "0qln" << '\n';

		// TODO: options

		std::cout << "uciok" << '\n';
	}

	void ExecuteCommand::IsReady()
	{
		std::cout << "readyok" << '\n';
	}

	void ExecuteCommand::UciNewGame(Machiavelli::Board* board)
	{
		*board = Machiavelli::Board::Board();
	}
}
