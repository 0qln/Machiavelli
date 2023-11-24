#include "BoardTests.h"

void BoardTests::GetSetPieces()
{
	Board b = Board::Board("8/8/8/8/8/8/8/8 w KQkq - 0 1");

	for (int i = Piece::WhiteNULL; i <= Piece::BlackKing; i++) {
		b.SetPiece(i, Piece(i));

		for (int j = PieceType::PT_NULL; j <= PieceType::King; j++) b.PrintPieceTypeBitboard(PieceType(j));
		b.PrintColorBitboard(Color::White);
		b.PrintColorBitboard(Color::Black);

		std::cout << "=>" << "\n";
		std::cout << b.ToString() << '\n';
		std::cout << "--------------" << "\n";
	}

	std::cout << b.GetPiece(0) << '\n';
	std::cout << b.GetPiece(1) << '\n';
}
