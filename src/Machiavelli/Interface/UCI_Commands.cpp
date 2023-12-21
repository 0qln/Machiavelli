#include "UCI_Commands.h"

#include "../Core/Misc.h"
#include "../Core/MoveHelper.h"
#include "../Core/Search.h"

#include <iostream>


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
		/* Save options, if the minimal work apprach appears to fail. 
		 * board = Machiavelli::Board::FromFEN(fen);  */
		
		 // only need to set up the fen, we can assume that `board` is cleared, because
		// the gui should always sent Commands::UCI_NEW_GAME to notify a reset, before 
		// requesting a position to be set up.
		board->SetUpFEN(fen);

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

	bool ExecuteCommand::Go(std::string task, std::vector<std::string> tokens, Machiavelli::Board* board)
	{

		if (task == "perft") {
			std::cout << '\n';
			Machiavelli::MoveGen::MoveGen(board).Perft(std::stoi(tokens[0]), true);
			std::cout << '\n';
			return true;
		}

		if (task == "depth") {
			auto depth = std::stoi(tokens[0]);
			Machiavelli::Search(board).Start(depth);
		}

		return false;
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
