#pragma once

#include <string>

#include "../Core/Misc.h"
#include "../Core/Board.h"


namespace CUSTOM {

	enum class Command {
		NONE = -1,
		STATIC_EVAL,
		PHASE,
	};


	/// <summary>
	/// 
	/// </summary>
	/// <param name="token"></param>
	/// <returns>UCI Command if a target was found, else -1.</returns>
	Command ParseCommand(const std::string& token);


	class ExecuteCommand {
	public:
		/// <summary>
		/// Print out the static evaluation of the current position.
		/// </summary>
		/// <param name="c">Relative to what player?</param>
		/// <param name="board">The current position</param>
		static void StaticEvaluation(Machiavelli::Board* board, Machiavelli::Color c = Machiavelli::Color::White);

		/// <summary>
		/// Respond with current phase of the game.
		/// </summary>
		/// <param name="board"></param>
		static void Phase(Machiavelli::Board* board);
	};
}
