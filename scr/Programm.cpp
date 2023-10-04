#include <iostream>
#include <string>

#include "Misc.hpp"
#include "Move.hpp"
#include "Board.hpp"


int main()
{
	Board b = Board::Board("8/8/2P5/8/8/2R5/8/8 w - - 0 1");


	std::cout << b.ToSring(b.RookAttacks());

}