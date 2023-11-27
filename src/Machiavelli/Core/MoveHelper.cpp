#include "pch.h"
#include "MoveHelper.h"


namespace Machiavelli {

    bool MoveHelper::IsQuietMove(const Move* move) {
        return (*move & Masks::FLAG_MASK) == FlagMasks::QUIET_MOVE_MASK;
    }

    bool MoveHelper::IsDoublePawnPush(const Move* move) {
        return (*move & Masks::FLAG_MASK) == FlagMasks::DOUBLE_PAWN_PUSH_MASK;
    }

    bool MoveHelper::IsCastle(const Move* move) {
        return IsKingSideCastle(move) || IsQueenSideCastle(move);
    }

    bool MoveHelper::IsKingSideCastle(const Move* move) {
        return (*move & Masks::FLAG_MASK) == FlagMasks::KING_CASTLE_MASK;
    }

    bool MoveHelper::IsQueenSideCastle(const Move* move) {
        return (*move & Masks::FLAG_MASK) == FlagMasks::QUEEN_CASTLE_MASK;
    }

    bool MoveHelper::IsCapture(const Move* move) {
        return IsOnlyCapture(move) || IsEnPassantCapture(move) || IsPromotionWithCapture(move);
    }

    bool MoveHelper::IsOnlyCapture(const Move* move) {
        return (*move & Masks::FLAG_MASK) == FlagMasks::CAPTURE_MASK;
    }

    bool MoveHelper::IsEnPassantCapture(const Move* move) {
        return (*move & Masks::FLAG_MASK) == FlagMasks::EN_PASSANT_MASK;
    }

    bool MoveHelper::IsPromotion(const Move* move) {
        return IsPromotionWithCapture(move) || IsPromotionWithoutCapture(move);
    }

    bool MoveHelper::IsPromotionWithCapture(const Move* move) {
        return
            IsKnightPromotionWithCapture(move) ||
            IsBishopPromotionWithCapture(move) ||
            IsRookPromotionWithCapture(move) ||
            IsQueenPromotionWithCapture(move);
    }

    bool MoveHelper::IsKnightPromotionWithCapture(const Move* move) {
        return (*move & Masks::FLAG_MASK) == FlagMasks::CAPTURE_PROMOTION_KNIGHT_MASK;
    }

    bool MoveHelper::IsBishopPromotionWithCapture(const Move* move) {
        return (*move & Masks::FLAG_MASK) == FlagMasks::CAPTURE_PROMOTION_BISHOP_MASK;
    }

    bool MoveHelper::IsRookPromotionWithCapture(const Move* move) {
        return (*move & Masks::FLAG_MASK) == FlagMasks::CAPTURE_PROMOTION_ROOK_MASK;
    }

    bool MoveHelper::IsQueenPromotionWithCapture(const Move* move) {
        return (*move & Masks::FLAG_MASK) == FlagMasks::CAPTURE_PROMOTION_QUEEN_MASK;
    }

    bool MoveHelper::IsPromotionWithoutCapture(const Move* move) {
        return IsKnightPromotion(move) || IsBishopPromotion(move) || IsRookPromotion(move) || IsQueenPromotion(move);
    }

    bool MoveHelper::IsKnightPromotion(const Move* move) {
        return (*move & Masks::FLAG_MASK) == FlagMasks::PROMOTION_KNIGHT_MASK;
    }

    bool MoveHelper::IsBishopPromotion(const Move* move) {
        return (*move & Masks::FLAG_MASK) == FlagMasks::PROMOTION_BISHOP_MASK;
    }

    bool MoveHelper::IsRookPromotion(const Move* move) {
        return (*move & Masks::FLAG_MASK) == FlagMasks::PROMOTION_ROOK_MASK;
    }

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

    std::string MoveHelper::ToString(Move move) {
        Square fromSqr = GetFrom(&move);
        Square toSqr = GetTo(&move);
        auto from = Misc::FromSquareIndex(fromSqr);
        auto to = Misc::FromSquareIndex(toSqr);
        from.append(to);
        return from;
    }
}