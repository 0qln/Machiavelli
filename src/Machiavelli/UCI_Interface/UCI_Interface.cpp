#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <regex>

#include "../Core/Board.h"
#include "../Core/MoveGen.h"
#include "../Core/MoveHelper.h"
#include "../Core/Misc.h"

#include "UCI_Commands.h"




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
		} 
		while (tokens[start++] != "moves");
		
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
			if (input == "exit") {
				break;
			}

			UCI::Command command = UCI::ParseCommand(tokens[0]);

			switch (command)
			{
			case UCI::Command::POSITION:
				UCI::ExecuteCommand::Position(GetFen(tokens), GetMoves(tokens), &board);
				break;
			
			default:
				break;
			}
		}
		
		return EXIT_SUCCESS;
	}

