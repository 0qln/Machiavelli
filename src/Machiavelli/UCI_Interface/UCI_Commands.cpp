#include "UCI_Commands.h"
#include "../Core/MoveHelper.h"
#include <iostream>

namespace UCI {
	Command UCI::ParseCommand(const std::string& token) {
		const std::map<std::string, Command> map_commands = {
			{ "position", POSITION },
			{ "go", GO },
		};
		return map_commands.at(token);
	}

	bool ExecuteCommand::Position(std::string fen, std::vector<std::string> moves, Machiavelli::Board* board)
	{
		// Set up board
		*board = Machiavelli::Board::FromFEN(fen);

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

		return false;
	}
}
