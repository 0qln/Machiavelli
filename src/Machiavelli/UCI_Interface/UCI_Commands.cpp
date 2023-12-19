#include "UCI_Commands.h"
#include "../Core/MoveHelper.h"

namespace UCI {
	Command UCI::ParseCommand(const std::string& token) {
		const std::map<std::string, Command> map_commands = {
			{ "position", POSITION },
		};
		return map_commands.at(token);
	}

	bool ExecuteCommand::Position(std::string fen, std::vector<std::string> moves, Machiavelli::Board* board)
	{
		board->SetFromFEN(fen);

		board->Print();

		for (auto move : moves) {
			// Make move
			auto plegals = Machiavelli::MoveGen(board).GeneratePseudoLegalMoves();
			Machiavelli::Move* actualMove = __nullptr;
			for (auto m : plegals) {
				if (Machiavelli::MoveHelper::ToString(m) == move) { 
					actualMove = &m; 
					break;
				}
			}

			if (!actualMove) {
				return false;
			}

			board->MakeMove(actualMove);

			board->Print();
		}
		
		return true;
	}
}
