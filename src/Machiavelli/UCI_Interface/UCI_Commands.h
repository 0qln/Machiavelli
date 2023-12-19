#pragma once

#include <string>
#include <map>
#include <vector>

#include "../Core/Board.h"

/// https://gist.github.com/DOBRO/2592c6dad754ba67e6dcaec8c90165bf

namespace UCI {
	enum Command {
		POSITION
	};

	Command ParseCommand(const std::string& token);;

	class ExecuteCommand {
	public:
		/// <summary>
		/// <para>
		/// Set up the position described in fenstring on the internal board and
		/// play the moves on the internal chess board. 
		/// </para>
		/// <para>
		/// If the game was played from the start position the string "startpos" will be sent 
		/// Note : no "new" command is needed.However, if this position is from a different game than 
		/// the last position sent to the engine, the GUI should have sent a "ucinewgame" inbetween.
		/// </para>
		/// </summary>
		/// <param name="fen"></param>
		/// <param name="moves"></param>
		/// <param name="board"></param>
		/// <returns>Success</returns>
		static bool Position(std::string fen, std::vector<std::string> moves, Machiavelli::Board* board);
	};
}