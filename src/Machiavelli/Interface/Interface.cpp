
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <regex>
#include <thread>

#include "../Core/Board.h"
#include "../Core/MoveGen.h"
#include "../Core/MoveHelper.h"
#include "../Core/Misc.h"

#include "UCI_Commands.h"
#include "CUSTOM_Commands.h"


std::string GetFen(std::vector<std::string> tokens) {
	std::string fen;
	if (tokens[1] == "fen") {
		fen = "";
		int i = 2;
		while (true) {
			fen.append(tokens[i++]);
			if (!(i <= 8 && i < tokens.size() && tokens[i] != "moves")) break;
			fen.append(" ");
		}
	}
	else if (tokens[1] == "startposition") {
		fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	}
	return fen;
}
std::vector<std::string> GetMoves(std::vector<std::string> tokens) {
	int start = 2;
	do {
		if (start >= tokens.size()) return std::vector<std::string>();
	} while (tokens[start++] != "moves");

	return std::vector<std::string>(tokens.begin() + start, tokens.end());
}


int main(int argc, char** argv)
{
	std::string input;
	Machiavelli::Board board;


	while (true) {
		std::getline(std::cin, input);
		std::vector<std::string> tokens;
		std::regex re("\\S+");
		std::sregex_token_iterator begin(input.begin(), input.end(), re), end;
		std::copy(begin, end, std::back_inserter(tokens));

		if (tokens.empty()) {
			continue;
		}

		// UCI first: in case of a name collision, UCI should have priority.

		// The UCI protocol demands, that the engine must be responive at 
		// any given moment. 
		// -> Run `ExecuteCommand` functions on seperate thread.
		std::thread engineT;
		
		switch (UCI::ParseCommand(tokens[0]))
		{
		case UCI::Command::POSITION:
			engineT = std::thread(UCI::ExecuteCommand::Position, GetFen(tokens), GetMoves(tokens), &board);
			engineT.detach();
			continue;

		case UCI::Command::GO:
			engineT = std::thread(UCI::ExecuteCommand::Go, tokens[1], std::vector<std::string>(tokens.begin() + 2, tokens.end()), &board);
			engineT.detach();
			continue;

		case UCI::Command::QUIT:
			UCI::ExecuteCommand::Quit();
			continue;

		case UCI::Command::UCI:
			UCI::ExecuteCommand::Uci();
			continue;

		case UCI::Command::IS_READY:
			UCI::ExecuteCommand::IsReady();
			continue;

		case UCI::Command::UCI_NEW_GAME:
			UCI::ExecuteCommand::UciNewGame(&board);
			continue;

		case UCI::Command::NONE:
			goto custom_command;
		}

	custom_command:
		switch (CUSTOM::ParseCommand(tokens[0]))
		{

		case CUSTOM::Command::STATIC_EVAL:
			engineT = std::thread(CUSTOM::ExecuteCommand::StaticEvaluation, &board, 
				tokens.size() == 3 && tokens[1] == "color" && tokens[2] == "black" ? Machiavelli::Color::Black : Machiavelli::Color::White);
			engineT.detach();
			break;

		case CUSTOM::Command::PHASE:
			CUSTOM::ExecuteCommand::Phase(&board);
			break;

		case CUSTOM::Command::PRINT_BOARD:
			CUSTOM::ExecuteCommand::PrintBoard(&board);
			break;

		case CUSTOM::Command::PRINT_INFO:
			CUSTOM::ExecuteCommand::PrintInfo(&board, std::vector<std::string>(tokens.begin() + 1, tokens.end()));
			break;
		}
	}

	return EXIT_SUCCESS;
}