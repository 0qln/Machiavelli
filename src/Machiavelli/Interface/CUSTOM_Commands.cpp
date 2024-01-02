#include "CUSTOM_Commands.h"
#include "../Core/Eval.h"
#include "../Core/BitHelper.h"

#include <iostream>


namespace CUSTOM {
	Command ParseCommand(const std::string& token)
	{
		const std::map<std::string, Command> map_commands = {
			{ "staticeval", Command::STATIC_EVAL },
			{ "phase", Command::PHASE },
			{ "printboard", Command::PRINT_BOARD },
			{ "printinfo", Command::PRINT_INFO },
		};

		if (map_commands.find(token) == map_commands.end()) {
			return Command::NONE;
		}

		return map_commands.at(token);
	}

	void ExecuteCommand::StaticEvaluation(Machiavelli::Board* board, Machiavelli::Color c)
	{
		auto eval = Machiavelli::StaticEval::Evaluate(board, c);
		std::cout << "Static Evaluation: " << eval << '\n';
	}

	void ExecuteCommand::Phase(Machiavelli::Board* board)
	{
		auto phase = Machiavelli::StaticEval::Phase(board);
		std::cout << "Phase [0;256]: " << phase << '\n';
	}

	void ExecuteCommand::PrintBoard(Machiavelli::Board* board)
	{
		board->PrintBoard();
	}

	void ExecuteCommand::PrintInfo(Machiavelli::Board* board, std::vector<std::string> tokens)
	{
		auto contains = [tokens](std::string value) { return std::find(tokens.begin(), tokens.end(), value) != tokens.end(); };

		if (contains("check")) {

			auto check_str = [](Machiavelli::Check check) {
				if (check == Machiavelli::Check::Single) return "Single";
				if (check == Machiavelli::Check::Double) return "Double";
				else return "None";
			};

			std::cout << "Check: " << check_str(board->GetCheck()) << "\n";
		}

		if (contains("pin")) {
			std::cout << "Pinned pieces: " << '\n';
			Machiavelli::BitHelper::PrintBitboard(board->GetPinnedPieces());
		}
	}
}