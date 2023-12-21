#include "CUSTOM_Commands.h"
#include "../Core/Eval.h"

#include <iostream>


namespace CUSTOM {
	Command ParseCommand(const std::string& token)
	{
		const std::map<std::string, Command> map_commands = {
			{ "staticeval", Command::STATIC_EVAL },
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
}