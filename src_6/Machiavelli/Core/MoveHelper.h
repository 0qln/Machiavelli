#pragma once

#include "Misc.h"


namespace Machiavelli
{
	class MoveHelper
	{
	public:
		using Flag = unsigned int;
		using Shift = unsigned int;
		using Mask = unsigned short;

		enum Shifts : Shift {
			FROM_SHIFT = 0,
			TO_SHIFT = 6,
			FLAG_SHIFT = 12
		};

		enum Masks : Mask {
			FROM_MASK = 0b111111 << Shifts::FROM_SHIFT,
			TO_MASK = 0b111111 << Shifts::TO_SHIFT,
			FLAG_MASK = 0b1111 << Shifts::FLAG_SHIFT
		};

		enum Flags : Flag {
			QUIET_MOVE_FLAG,
			DOUBLE_PAWN_PUSH_FLAG,
			PROMOTION_KNIGHT_FLAG,
			PROMOTION_BISHOP_FLAG,
			PROMOTION_ROOK_FLAG,
			PROMOTION_QUEEN_FLAG,
			CAPTURE_PROMOTION_KNIGHT_FLAG,
			CAPTURE_PROMOTION_BISHOP_FLAG,
			CAPTURE_PROMOTION_ROOK_FLAG,
			CAPTURE_PROMOTION_QUEEN_FLAG,
			KING_CASTLE_FLAG,
			QUEEN_CASTLE_FLAG,
			CAPTURE_FLAG,
			EN_PASSANT_FLAG,
		};

		enum FlagMasks : Mask {
			QUIET_MOVE_MASK = Flags::QUIET_MOVE_FLAG << Shifts::FLAG_SHIFT,
			DOUBLE_PAWN_PUSH_MASK = Flags::DOUBLE_PAWN_PUSH_FLAG << Shifts::FLAG_SHIFT,
			KING_CASTLE_MASK = Flags::KING_CASTLE_FLAG << Shifts::FLAG_SHIFT,
			QUEEN_CASTLE_MASK = Flags::QUEEN_CASTLE_FLAG << Shifts::FLAG_SHIFT,
			CAPTURE_MASK = Flags::CAPTURE_FLAG << Shifts::FLAG_SHIFT,
			EN_PASSANT_MASK = Flags::EN_PASSANT_FLAG << Shifts::FLAG_SHIFT,
			PROMOTION_KNIGHT_MASK = Flags::PROMOTION_KNIGHT_FLAG << Shifts::FLAG_SHIFT,
			PROMOTION_BISHOP_MASK = Flags::PROMOTION_BISHOP_FLAG << Shifts::FLAG_SHIFT,
			PROMOTION_ROOK_MASK = Flags::PROMOTION_ROOK_FLAG << Shifts::FLAG_SHIFT,
			PROMOTION_QUEEN_MASK = Flags::PROMOTION_QUEEN_FLAG << Shifts::FLAG_SHIFT,
			CAPTURE_PROMOTION_KNIGHT_MASK = Flags::CAPTURE_PROMOTION_KNIGHT_FLAG << Shifts::FLAG_SHIFT,
			CAPTURE_PROMOTION_BISHOP_MASK = Flags::CAPTURE_PROMOTION_BISHOP_FLAG << Shifts::FLAG_SHIFT,
			CAPTURE_PROMOTION_ROOK_MASK = Flags::CAPTURE_PROMOTION_ROOK_FLAG << Shifts::FLAG_SHIFT,
			CAPTURE_PROMOTION_QUEEN_MASK = Flags::CAPTURE_PROMOTION_QUEEN_FLAG << Shifts::FLAG_SHIFT
		};



		static bool IsQuietMove(const Move* move);
		static bool IsDoublePawnPush(const Move* move);
		static bool IsCastle(const Move* move);
		static bool IsKingSideCastle(const Move* move);
		static bool IsQueenSideCastle(const Move* move);
		static bool IsCapture(const Move* move);
		static bool IsOnlyCapture(const Move* move);
		static bool IsEnPassantCapture(const Move* move);
		static bool IsPromotion(const Move* move);
		static bool IsPromotionWithCapture(const Move* move);
		static bool IsKnightPromotionWithCapture(const Move* move);
		static bool IsBishopPromotionWithCapture(const Move* move);
		static bool IsRookPromotionWithCapture(const Move* move);
		static bool IsQueenPromotionWithCapture(const Move* move);
		static bool IsPromotionWithoutCapture(const Move* move);
		static bool IsKnightPromotion(const Move* move);
		static bool IsBishopPromotion(const Move* move);
		static bool IsRookPromotion(const Move* move);
		static bool IsQueenPromotion(const Move* move);
		static Square GetTo(const Move* move);
		static Square GetFrom(const Move* move);
		static Flag GetFlag(const Move* move);
		static Move Create(const Square from, const Square to, const Flag flag);
		static Move Create(const std::string str, const Flag flag);
		static std::string ToString(Move move);
	};


}