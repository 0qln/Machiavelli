#pragma once

#include "Misc.hpp"

class MoveHelper
{
public:
	const static unsigned int FROM_SHIFT =		0;
	const static unsigned int TO_SHIFT =		6;
	const static unsigned int FLAG_SHIFT =	12;

	const static Move FROM_MASK =	0b111111 << FROM_SHIFT;
	const static Move TO_MASK =		0b111111 << TO_SHIFT;
	const static Move FLAG_MASK =	0b1111 << FLAG_SHIFT;

	const static unsigned int QUITE_MOVE_FLAG =					0;
	const static unsigned int DOUBLE_PAWN_PUSH_FLAG =			1;
	const static unsigned int KING_CASTLE_FLAG =				2;
	const static unsigned int QUEEN_CASTLE_FLAG =				3;
	const static unsigned int CAPTURE_FLAG =					4;
	const static unsigned int EN_PASSANT_FLAG =					5;
	const static unsigned int PROMOTION_KNIGHT_FLAG =			8;
	const static unsigned int PROMOTION_BISHOP_FLAG =			9;
	const static unsigned int PROMOTION_ROOK_FLAG =				10;
	const static unsigned int PROMOTION_QUEEN_FLAG =			11;
	const static unsigned int CAPTURE_PROMOTION_KNIGHT_FLAG =	12;
	const static unsigned int CAPTURE_PROMOTION_BISHOP_FLAG =	13;
	const static unsigned int CAPTURE_PROMOTION_ROOK_FLAG =		14;
	const static unsigned int CAPTURE_PROMOTION_QUEEN_FLAG =	15;

	const static Move QUITE_MOVE_MASK =						QUITE_MOVE_FLAG					<< FLAG_SHIFT;
	const static Move DOUBLE_PAWN_PUSH_MASK =				DOUBLE_PAWN_PUSH_FLAG			<< FLAG_SHIFT;
	const static Move KING_CASTLE_MASK =					KING_CASTLE_FLAG				<< FLAG_SHIFT;
	const static Move QUEEN_CASTLE_MASK =					QUEEN_CASTLE_FLAG				<< FLAG_SHIFT;
	const static Move CAPTURE_MASK =						CAPTURE_FLAG					<< FLAG_SHIFT;
	const static Move EN_PASSANT_MASK =						EN_PASSANT_FLAG					<< FLAG_SHIFT;
	const static Move PROMOTION_KNIGHT_MASK =				PROMOTION_KNIGHT_FLAG			<< FLAG_SHIFT;
	const static Move PROMOTION_BISHOP_MASK =				PROMOTION_BISHOP_FLAG			<< FLAG_SHIFT;
	const static Move PROMOTION_ROOK_MASK =					PROMOTION_ROOK_FLAG				<< FLAG_SHIFT;
	const static Move PROMOTION_QUEEN_MASK =				PROMOTION_QUEEN_FLAG			<< FLAG_SHIFT;
	const static Move CAPTURE_PROMOTION_KNIGHT_MASK =		CAPTURE_PROMOTION_KNIGHT_FLAG	<< FLAG_SHIFT;
	const static Move CAPTURE_PROMOTION_BISHOP_MASK =		CAPTURE_PROMOTION_BISHOP_FLAG	<< FLAG_SHIFT;
	const static Move CAPTURE_PROMOTION_ROOK_MASK =			CAPTURE_PROMOTION_ROOK_FLAG		<< FLAG_SHIFT;
	const static Move CAPTURE_PROMOTION_QUEEN_MASK =		CAPTURE_PROMOTION_QUEEN_FLAG	<< FLAG_SHIFT;



	static inline bool IsQuiteMove(const Move* move) {
		return (*move & FLAG_MASK) == QUITE_MOVE_MASK;
	}

	static inline bool IsDoublePawnPush(const Move* move) {
		return (*move & FLAG_MASK) == DOUBLE_PAWN_PUSH_MASK;
	}

	static inline bool IsCastle(const Move* move) {
		return IsKingSideCastle(move)
			|| IsQueenSideCastle(move);
	}
	static inline bool IsKingSideCastle(const Move* move) {
		return (*move & FLAG_MASK) == KING_CASTLE_MASK;
	}
	static inline bool IsQueenSideCastle(const Move* move) {
		return (*move & FLAG_MASK) == QUEEN_CASTLE_MASK;
	}

	static inline bool IsCapture(const Move* move) {
		return IsOnlyCapture(move)
			|| IsEnPassantCapture(move)
			|| IsPromotionWithCapture(move);
	}
	static inline bool IsOnlyCapture(const Move* move) {
		return (*move & FLAG_MASK) == CAPTURE_MASK;
	}
	static inline bool IsEnPassantCapture(const Move* move) {
		return (*move & FLAG_MASK) == EN_PASSANT_MASK;
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
		return (*move & FLAG_MASK) == CAPTURE_PROMOTION_KNIGHT_MASK;
	}
	static inline bool IsBishopPromotionWithCapture(const Move* move) {
		return (*move & FLAG_MASK) == CAPTURE_PROMOTION_BISHOP_MASK;
	}
	static inline bool IsRookPromotionWithCapture(const Move* move) {
		return (*move & FLAG_MASK) == CAPTURE_PROMOTION_ROOK_MASK;
	}
	static inline bool IsQueenPromotionWithCapture(const Move* move) {
		return (*move & FLAG_MASK) == CAPTURE_PROMOTION_QUEEN_MASK;
	}

	static inline bool IsPromotionWithoutCapture(const Move* move) {
		return IsKnightPromotion(move)
			|| IsBishopPromotion(move)
			|| IsRookPromotion(move)
			|| IsQueenPromotion(move);
	}
	static inline bool IsKnightPromotion(const Move* move) {
		return (*move & FLAG_MASK) == PROMOTION_KNIGHT_MASK;
	}
	static inline bool IsBishopPromotion(const Move* move) {
		return (*move & FLAG_MASK) == PROMOTION_BISHOP_MASK;
	}
	static inline bool IsRookPromotion(const Move* move) {
		return (*move & FLAG_MASK) == PROMOTION_ROOK_MASK;
	}
	static inline bool IsQueenPromotion(const Move* move) {
		return (*move & FLAG_MASK) == PROMOTION_QUEEN_MASK;
	}


	static Square GetTo(const Move* move) {
		return (*move & TO_MASK) >> TO_SHIFT;
	}
	static Square GetFrom(const Move* move) {
		return (*move & FROM_MASK) >> FROM_SHIFT;
	}
	static unsigned __int8 GetFlag(const Move* move) {
		return (*move & FLAG_MASK) >> FLAG_SHIFT;
	}

	static Move Create(Square from, Square to, unsigned __int64 flags) {
		return (flags << FLAG_SHIFT) | (Bitboard(to) << TO_SHIFT) | (Bitboard(from) << FROM_SHIFT);
	}


};