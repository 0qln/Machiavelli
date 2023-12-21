#include "pch.h"
#include "Eval.h"
#include "BitHelper.h"

namespace Machiavelli {

    const Score StaticEval::PieceValues[7]{
        0, 100, 300, 350, 500, 850, 9999
    };

    Score StaticEval::Evaluate(Board* board, Color color)
    {
        Score result = 0;

        // Simply material evaluation as placeholder.

        for (int pt = PieceType::Pawn; pt <= PieceType::Queen; pt++) {
            // ally
            result += PieceValues[pt] * BitHelper::CountBits(
                board->GetPieceBitboard(PieceType(pt)) &
                board->GetColorBitboard(color));

            // enemey
            result -= PieceValues[pt] * BitHelper::CountBits(
                board->GetPieceBitboard(PieceType(pt)) &
                board->GetColorBitboard(Color(!color)));
        }

        return result;
    }
}
