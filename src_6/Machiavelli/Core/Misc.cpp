#include "pch.h"
#include "Misc.h"

namespace Machiavelli {
	Square Misc::ToSquareIndex(const std::string* square)
	{
		File file = square->at(0) - 'a';
		Rank rank = square->at(1) - '1';
		return SquareIndex0(rank, file);
	}

	std::string Misc::FromSquareIndex(const Square square)
	{
		char file = (square % 8) + 'a';
		char rank = (square / 8) + '1';
		return std::string({ file, rank });
	}

	Square Misc::SquareIndex1(const Rank rank, const File file)
	{
		return file + 8 * (rank - 1) - 1;
	}

	Square Misc::SquareIndex0(const Rank rank, const File file)
	{
		return file + 8 * rank;
	}
}