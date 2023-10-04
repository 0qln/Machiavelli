#include <iostream>
#include <string>

#include "Misc.hpp"
#include "Move.hpp"
#include "Board.hpp"


int main()
{
	Board b = Board::Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	Move move = MoveHelper::Create(0, 1, MoveHelper::CAPTURE_PROMOTION_BISHOP_FLAG);

	std::cout << MoveHelper::IsBishopPromotionWithCapture(&move);
	std::cout << MoveHelper::IsBishopPromotion(&move);
	std::cout << MoveHelper::IsCapture(&move);

	std::cout << b.ToSring();
}