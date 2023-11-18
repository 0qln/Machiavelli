#pragma once

#include "Misc.hpp"


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
		TO_MASK =	0b111111 << Shifts::TO_SHIFT,
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
		QUITE_MOVE_MASK					= Flags::QUIET_MOVE_FLAG				<< Shifts::FLAG_SHIFT,
		DOUBLE_PAWN_PUSH_MASK			= Flags::DOUBLE_PAWN_PUSH_FLAG			<< Shifts::FLAG_SHIFT,
		KING_CASTLE_MASK				= Flags::KING_CASTLE_FLAG				<< Shifts::FLAG_SHIFT,
		QUEEN_CASTLE_MASK				= Flags::QUEEN_CASTLE_FLAG				<< Shifts::FLAG_SHIFT,
		CAPTURE_MASK					= Flags::CAPTURE_FLAG					<< Shifts::FLAG_SHIFT,
		EN_PASSANT_MASK					= Flags::EN_PASSANT_FLAG				<< Shifts::FLAG_SHIFT,
		PROMOTION_KNIGHT_MASK			= Flags::PROMOTION_KNIGHT_FLAG			<< Shifts::FLAG_SHIFT,
		PROMOTION_BISHOP_MASK			= Flags::PROMOTION_BISHOP_FLAG			<< Shifts::FLAG_SHIFT,
		PROMOTION_ROOK_MASK				= Flags::PROMOTION_ROOK_FLAG			<< Shifts::FLAG_SHIFT,
		PROMOTION_QUEEN_MASK			= Flags::PROMOTION_QUEEN_FLAG			<< Shifts::FLAG_SHIFT,
		CAPTURE_PROMOTION_KNIGHT_MASK	= Flags::CAPTURE_PROMOTION_KNIGHT_FLAG	<< Shifts::FLAG_SHIFT,
		CAPTURE_PROMOTION_BISHOP_MASK	= Flags::CAPTURE_PROMOTION_BISHOP_FLAG	<< Shifts::FLAG_SHIFT,
		CAPTURE_PROMOTION_ROOK_MASK		= Flags::CAPTURE_PROMOTION_ROOK_FLAG	<< Shifts::FLAG_SHIFT,
		CAPTURE_PROMOTION_QUEEN_MASK	= Flags::CAPTURE_PROMOTION_QUEEN_FLAG	<< Shifts::FLAG_SHIFT
	};




	//TODO: maybe cache the results? and let the current
	//		move being evaluated be stored in a global field
	//		that can has to be set before evaluating
	// 
	//		ex: 
	//		MoveHelper::EvaluateMove = &myMove;
	//		bool isquiet = EvaluateMove::IsQuiet();
	// 
	//		would have to perform a pert to
	//		see wether it's worth

	static inline bool IsQuietMove(const Move* move) {
		return (*move & Masks::FLAG_MASK) == FlagMasks::QUITE_MOVE_MASK;
	}

	static inline bool IsDoublePawnPush(const Move* move) {
		return (*move & Masks::FLAG_MASK) == FlagMasks::DOUBLE_PAWN_PUSH_MASK;
	}

	static inline bool IsCastle(const Move* move) {
		return IsKingSideCastle(move)
			|| IsQueenSideCastle(move);
	}
	static inline bool IsKingSideCastle(const Move* move) {
		return (*move & Masks::FLAG_MASK) == FlagMasks::KING_CASTLE_MASK;
	}
	static inline bool IsQueenSideCastle(const Move* move) {
		return (*move & Masks::FLAG_MASK) == FlagMasks::QUEEN_CASTLE_MASK;
	}

	static inline bool IsCapture(const Move* move) {
		return IsOnlyCapture(move)
			|| IsEnPassantCapture(move)
			|| IsPromotionWithCapture(move);
	}
	static inline bool IsOnlyCapture(const Move* move) {
		return (*move & Masks::FLAG_MASK) == FlagMasks::CAPTURE_MASK;
	}
	static inline bool IsEnPassantCapture(const Move* move) {
		return (*move & Masks::FLAG_MASK) == FlagMasks::EN_PASSANT_MASK;
	}

	static inline bool IsPromotion(const Move* move) {
		return IsPromotionWithCapture(move)
			|| IsPromotionWithoutCapture(move);
	}

	static inline bool IsPromotionWithCapture(const Move* move) {
		return IsKnightPromotionWithCapture(move)
			|| IsBishopPromotionWithCapture(move)
			|| IsRookPromotionWithCapture(move)
			|| IsQueenPromotionWithCapture(move);
	}
	static inline bool IsKnightPromotionWithCapture(const Move* move) {
		return (*move & Masks::FLAG_MASK) == FlagMasks::CAPTURE_PROMOTION_KNIGHT_MASK;
	}
	static inline bool IsBishopPromotionWithCapture(const Move* move) {
		return (*move & Masks::FLAG_MASK) == FlagMasks::CAPTURE_PROMOTION_BISHOP_MASK;
	}
	static inline bool IsRookPromotionWithCapture(const Move* move) {
		return (*move & Masks::FLAG_MASK) == FlagMasks::CAPTURE_PROMOTION_ROOK_MASK;
	}
	static inline bool IsQueenPromotionWithCapture(const Move* move) {
		return (*move & Masks::FLAG_MASK) == FlagMasks::CAPTURE_PROMOTION_QUEEN_MASK;
	}

	static inline bool IsPromotionWithoutCapture(const Move* move) {
		return IsKnightPromotion(move)
			|| IsBishopPromotion(move)
			|| IsRookPromotion(move)
			|| IsQueenPromotion(move);
	}
	static inline bool IsKnightPromotion(const Move* move) {
		return (*move & Masks::FLAG_MASK) == FlagMasks::PROMOTION_KNIGHT_MASK;
	}
	static inline bool IsBishopPromotion(const Move* move) {
		return (*move & Masks::FLAG_MASK) == FlagMasks::PROMOTION_BISHOP_MASK;
	}
	static inline bool IsRookPromotion(const Move* move) {
		return (*move & Masks::FLAG_MASK) == FlagMasks::PROMOTION_ROOK_MASK;
	}
	static inline bool IsQueenPromotion(const Move* move) {
		return (*move & Masks::FLAG_MASK) == FlagMasks::PROMOTION_QUEEN_MASK;
	}


	static Square GetTo(const Move* move) {
		return (*move & Masks::TO_MASK) >> Shifts::TO_SHIFT;
	}
	static Square GetFrom(const Move* move) {
		return (*move & Masks::FROM_MASK) >> Shifts::FROM_SHIFT;
	}
	static Flag GetFlag(const Move* move) {
		return (*move & Masks::FLAG_MASK) >> Shifts::FLAG_SHIFT;
	}

	static Move Create(const Square from, const Square to, const Flag flag) {
		return (Bitboard(flag) << Shifts::FLAG_SHIFT) | (Bitboard(to) << Shifts::TO_SHIFT) | (Bitboard(from) << Shifts::FROM_SHIFT);
	}

	// input ex: "e2e4"
	static Move Create(const std::string str, const Flag flag) {
		auto from = str.substr(0, 2);
		auto to = str.substr(2, 2);
		Square fromSqr = Misc::ToSquareIndex(&from);
		Square toSqr = Misc::ToSquareIndex(&to);
		return Create(fromSqr, toSqr, flag);
	}


};