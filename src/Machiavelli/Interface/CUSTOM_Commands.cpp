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

		std::cout << '\n';

		auto all = contains("all");

		if (all || contains("board")) {
			std::cout << "Board: " << '\n';
			board->PrintBoard();
		}

		if (all || contains("check")) {
			//std::cout << "Check: " << '\n';
			auto check_str = [](Machiavelli::Check check) {
				if (check == Machiavelli::Check::Single) return "Single";
				if (check == Machiavelli::Check::Double) return "Double";
				else return "None";
			};

			std::cout << "Check: " << check_str(board->GetCheck()) << "\n\n";
		}

		if (all || contains("pin")) {
			std::cout << "Pinned pieces [White]: " << '\n';
			Machiavelli::BitHelper::PrintBitboard(board->GetPinnedPieces(Machiavelli::White));
			std::cout << "Pinned pieces [Black]: " << '\n';
			Machiavelli::BitHelper::PrintBitboard(board->GetPinnedPieces(Machiavelli::Black));
		}

		if (all || contains("checkblockades")) {
			std::cout << "Check blockades: " << '\n';
			Machiavelli::BitHelper::PrintBitboard(board->GetCheckBlockades());
		}

		if (all || contains("attacks")) {
			std::cout << "Attacks [White]: " << '\n';
			Machiavelli::BitHelper::PrintBitboard(board->GetAttacks(Machiavelli::White));
			std::cout << "Attacks [Black]: " << '\n';
			Machiavelli::BitHelper::PrintBitboard(board->GetAttacks(Machiavelli::Black));
		}

		if (all || contains("pieces")) {
			std::cout << "Pieces [White]: " << '\n';
			Machiavelli::BitHelper::PrintBitboard(board->GetColorBitboard(Machiavelli::White));
			std::cout << "Pieces [Black]: " << '\n';
			Machiavelli::BitHelper::PrintBitboard(board->GetColorBitboard(Machiavelli::Black));
		}

		if (all || contains("piecetypes")) {
			std::cout << "Pieces [Pawn]: " << '\n';
			Machiavelli::BitHelper::PrintBitboard(board->GetPieceBitboard(Machiavelli::Pawn));
			std::cout << "Pieces [Knight]: " << '\n';
			Machiavelli::BitHelper::PrintBitboard(board->GetPieceBitboard(Machiavelli::Knight));
			std::cout << "Pieces [Bishop]: " << '\n';
			Machiavelli::BitHelper::PrintBitboard(board->GetPieceBitboard(Machiavelli::Bishop));
			std::cout << "Pieces [Rook]: " << '\n';
			Machiavelli::BitHelper::PrintBitboard(board->GetPieceBitboard(Machiavelli::Rook));
			std::cout << "Pieces [Queen]: " << '\n';
			Machiavelli::BitHelper::PrintBitboard(board->GetPieceBitboard(Machiavelli::Queen));
			std::cout << "Pieces [King]: " << '\n';
			Machiavelli::BitHelper::PrintBitboard(board->GetPieceBitboard(Machiavelli::King));
		}

		if (all || contains("statecache")) {

		}

		if (all || contains("enpassant")) {
			std::cout << "Possible en passant capture: " << '\n';
			std::cout << Machiavelli::Misc::FromSquareIndex(board->GetEnPassantSquare()) << '\n';
			Machiavelli::BitHelper::PrintBitboard(board->GetEnPassantBitboard());
		}
	}
}