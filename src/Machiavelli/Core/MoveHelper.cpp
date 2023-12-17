#include "pch.h"
#include "MoveHelper.h"


namespace Machiavelli {

    /// <summary>
    /// Check wether the move is a quiet move or not
    /// </summary>
    /// <param name="move">A pointer to the move</param>
    /// <returns></returns>
    bool MoveHelper::IsQuietMove(const Move* move) {
        return (*move & Masks::FLAG_MASK) == FlagMasks::QUIET_MOVE_MASK;
    }


    /// <summary>
    /// Check wether the move is a double pawn push or not
    /// </summary>
    /// <param name="move">A pointer to the move</param>
    /// <returns></returns>
    bool MoveHelper::IsDoublePawnPush(const Move* move) {
        return (*move & Masks::FLAG_MASK) == FlagMasks::DOUBLE_PAWN_PUSH_MASK;
    }


    /// <summary>
    /// Check wether the move is a castling move or not
    /// </summary>
    /// <param name="move">A pointer to the move</param>
    /// <returns></returns>
    bool MoveHelper::IsCastle(const Move* move) {
        return IsKingSideCastle(move) || IsQueenSideCastle(move);
    }


    /// <summary>
    /// Check wether the move is a king side castle or not
    /// </summary>
    /// <param name="move">A pointer to the move</param>
    /// <returns></returns>
    bool MoveHelper::IsKingSideCastle(const Move* move) {
        return (*move & Masks::FLAG_MASK) == FlagMasks::KING_CASTLE_MASK;
    }


    /// <summary>
    /// Check wether the move is a queen side castle or not
    /// </summary>
    /// <param name="move">A pointer to the move</param>
    /// <returns></returns>
    bool MoveHelper::IsQueenSideCastle(const Move* move) {
        return (*move & Masks::FLAG_MASK) == FlagMasks::QUEEN_CASTLE_MASK;
    }


    /// <summary>
    /// Check wether the move is a capturing move or not
    /// </summary>
    /// <param name="move">A pointer to the move</param>
    /// <returns></returns>
    bool MoveHelper::IsCapture(const Move* move) {
        return IsOnlyCapture(move) || IsEnPassantCapture(move) || IsPromotionWithCapture(move);
    }


    /// <summary>
    /// Check wether the move is a capturing move and nothing else or not
    /// </summary>
    /// <param name="move">A pointer to the move</param>
    /// <returns></returns>
    bool MoveHelper::IsOnlyCapture(const Move* move) {
        return (*move & Masks::FLAG_MASK) == FlagMasks::CAPTURE_MASK;
    }


    /// <summary>
    /// Check wether the move is an en passant capture or not
    /// </summary>
    /// <param name="move">A pointer to the move</param>
    /// <returns></returns>
    bool MoveHelper::IsEnPassantCapture(const Move* move) {
        return (*move & Masks::FLAG_MASK) == FlagMasks::EN_PASSANT_MASK;
    }


    /// <summary>
    /// Check wether the move is generally a promotion or not
    /// </summary>
    /// <param name="move">A pointer to the move</param>
    /// <returns></returns>
    bool MoveHelper::IsPromotion(const Move* move) {
        return IsPromotionWithCapture(move) || IsPromotionWithoutCapture(move);
    }


    /// <summary>
    /// Check wether the move is generally a promotion that also captures something or not
    /// </summary>
    /// <param name="move">A pointer to the move</param>
    /// <returns></returns>
    bool MoveHelper::IsPromotionWithCapture(const Move* move) {
        return
            IsKnightPromotionWithCapture(move) ||
            IsBishopPromotionWithCapture(move) ||
            IsRookPromotionWithCapture(move) ||
            IsQueenPromotionWithCapture(move);
    }


    /// <summary>
    /// Check wether the move is a promotion to a knight that also captures something or not
    /// </summary>
    /// <param name="move">A pointer to the move</param>
    /// <returns></returns>
    bool MoveHelper::IsKnightPromotionWithCapture(const Move* move) {
        return (*move & Masks::FLAG_MASK) == FlagMasks::CAPTURE_PROMOTION_KNIGHT_MASK;
    }


    /// <summary>
    /// Check wether the move is a promotion to a bishop that also captures something or not
    /// </summary>
    /// <param name="move">A pointer to the move</param>
    /// <returns></returns>
    bool MoveHelper::IsBishopPromotionWithCapture(const Move* move) {
        return (*move & Masks::FLAG_MASK) == FlagMasks::CAPTURE_PROMOTION_BISHOP_MASK;
    }


    /// <summary>
    /// Check wether the move is a promotion to a rook that also captures something or not
    /// </summary>
    /// <param name="move">A pointer to the move</param>
    /// <returns></returns>
    bool MoveHelper::IsRookPromotionWithCapture(const Move* move) {
        return (*move & Masks::FLAG_MASK) == FlagMasks::CAPTURE_PROMOTION_ROOK_MASK;
    }


    /// <summary>
    /// Check wether the move is a promotion to a queen that also captures something or not
    /// </summary>
    /// <param name="move">A pointer to the move</param>
    /// <returns></returns>
    bool MoveHelper::IsQueenPromotionWithCapture(const Move* move) {
        return (*move & Masks::FLAG_MASK) == FlagMasks::CAPTURE_PROMOTION_QUEEN_MASK;
    }


    /// <summary>
    /// Check wether the move is a promotion to a queen that also captures something or not
    /// </summary>
    /// <param name="move">A pointer to the move</param>
    /// <returns></returns>
    bool MoveHelper::IsPromotionWithoutCapture(const Move* move) {
        return IsKnightPromotion(move) || IsBishopPromotion(move) || IsRookPromotion(move) || IsQueenPromotion(move);
    }


    /// <summary>
    /// Check wether the move is a promotion to a knight or not
    /// </summary>
    /// <param name="move">A pointer to the move</param>
    /// <returns></returns>
    bool MoveHelper::IsKnightPromotion(const Move* move) {
        return (*move & Masks::FLAG_MASK) == FlagMasks::PROMOTION_KNIGHT_MASK;
    }


    /// <summary>
    /// Check wether the move is a promotion to a bishop or not
    /// </summary>
    /// <param name="move">A pointer to the move</param>
    /// <returns></returns>
    bool MoveHelper::IsBishopPromotion(const Move* move) {
        return (*move & Masks::FLAG_MASK) == FlagMasks::PROMOTION_BISHOP_MASK;
    }


    /// <summary>
    /// Check wether the move is a promotion to a rook or not
    /// </summary>
    /// <param name="move">A pointer to the move</param>
    /// <returns></returns>
    bool MoveHelper::IsRookPromotion(const Move* move) {
        return (*move & Masks::FLAG_MASK) == FlagMasks::PROMOTION_ROOK_MASK;
    }


    /// <summary>
    /// Check wether the move is a promotion to a queen or not
    /// </summary>
    /// <param name="move">A pointer to the move</param>
    /// <returns></returns>
    bool MoveHelper::IsQueenPromotion(const Move* move) {
        return (*move & Masks::FLAG_MASK) == FlagMasks::PROMOTION_QUEEN_MASK;
    }


    Square MoveHelper::GetTo(const Move* move) {
        return (*move & Masks::TO_MASK) >> Shifts::TO_SHIFT;
    }

    Square MoveHelper::GetFrom(const Move* move) {
        return (*move & Masks::FROM_MASK) >> Shifts::FROM_SHIFT;
    }

    MoveHelper::Flag MoveHelper::GetFlag(const Move* move) {
        return (*move & Masks::FLAG_MASK) >> Shifts::FLAG_SHIFT;
    }

    Move MoveHelper::Create(const Square from, const Square to, const Flag flag) {
        return (Bitboard(flag) << Shifts::FLAG_SHIFT) | (Bitboard(to) << Shifts::TO_SHIFT) |
            (Bitboard(from) << Shifts::FROM_SHIFT);
    }

    Move MoveHelper::Create(const std::string str, const Flag flag) {
        auto from = str.substr(0, 2);
        auto to = str.substr(2, 2);
        Square fromSqr = Misc::ToSquareIndex(&from);
        Square toSqr = Misc::ToSquareIndex(&to);
        return Create(fromSqr, toSqr, flag);
    }

    /// <summary>
    /// Convert a move to algebraic notation.
    /// </summary>
    /// <param name="move">The move to be converted</param>
    /// <returns></returns>
    std::string MoveHelper::ToString(const Move move) {
        Square fromSqr = GetFrom(&move);
        Square toSqr = GetTo(&move);
        auto from = Misc::FromSquareIndex(fromSqr);
        auto to = Misc::FromSquareIndex(toSqr);
        from.append(to);
        from.append("-");
        from.append(std::to_string(int(GetFlag(&move))));
        if (GetFlag(&move) < 10) from.append(" "); // right pad
        return from;
    }
}