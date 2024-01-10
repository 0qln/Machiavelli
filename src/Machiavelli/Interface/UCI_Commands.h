#pragma once

#include <string>
#include <map>
#include <vector>
#include <optional>

#include "../Core/Board.h"
#include "../Core/Search.h"


namespace UCI {

	enum class Command {
		NONE = -1,
		POSITION,
		GO,
		QUIT,
		UCI,
		IS_READY,
		UCI_NEW_GAME,
		STOP
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

		/// <summary>
		/// Start calculating on the current position set up with the "position" command.
		/// </summary>
		/// <param name="task"></param>
		/// <param name="tokens"></param>
		/// <param name="board"></param>
		/// <returns></returns>
		static void Go(std::string task, std::vector<std::string> tokens, Machiavelli::Board* board, std::optional<Machiavelli::SearchCancelationToken*>* outCancelationToken);

		/// <summary>
		/// Quit the program as soon as possible.
		/// </summary>
		static void Quit();

		/// <summary>
		/// 1. Identify with 'id' response.
		/// 2. Notify the GUI of available options with 'option' response.
		/// 3. Sent 'uciok' response.
		/// </summary>
		static void Uci();

		/// <summary>
		/// Sends "readyok" response.
		/// </summary>
		static void IsReady();

		/// <summary>
		/// Makes preperations for a new game. 
		/// Expecting to receive a `Position` command shortly after. 
		/// (Protocol statates that this command should not be relied on tho)
		/// </summary>
		static void UciNewGame(Machiavelli::Board* board);

		static void Stop(Machiavelli::Board* board, std::optional<Machiavelli::SearchCancelationToken*> cancelationToken);
	};
}