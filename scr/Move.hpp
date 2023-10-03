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



	static inline bool IsQuiteMove(Move move) {
		return (move & FLAG_MASK) == QUITE_MOVE_MASK;
	}

	static inline bool IsDoublePawnPush(Move move) {
		return (move & FLAG_MASK) == DOUBLE_PAWN_PUSH_MASK;
	}

	static inline bool IsKingSideCastle(Move move) {
		return (move & FLAG_MASK) == KING_CASTLE_MASK;
	}
	static inline bool IsQueenSideCastle(Move move) {
		return (move & FLAG_MASK) == QUEEN_CASTLE_MASK;
	}

	static inline bool IsCapture(Move move) {
		return (move & FLAG_MASK) == CAPTURE_MASK;
	}
	static inline bool IsEnPassantCapture(Move move) {
		return (move & FLAG_MASK) == EN_PASSANT_MASK;
	}

	static inline bool IsKnightPromotion(Move move) {
		return (move & FLAG_MASK) == PROMOTION_KNIGHT_MASK;
	}
	static inline bool IsBishopPromotion(Move move) {
		return (move & FLAG_MASK) == PROMOTION_BISHOP_MASK;
	}
	static inline bool IsRookPromotion(Move move) {
		return (move & FLAG_MASK) == PROMOTION_ROOK_MASK;
	}
	static inline bool IsQueenPromotion(Move move) {
		return (move & FLAG_MASK) == PROMOTION_QUEEN_MASK;
	}

	static inline bool IsKnightPromotionWithCapture(Move move) {
		return (move & FLAG_MASK) == CAPTURE_PROMOTION_KNIGHT_MASK;
	}
	static inline bool IsBishopPromotionWithCapture(Move move) {
		return (move & FLAG_MASK) == CAPTURE_PROMOTION_BISHOP_MASK;
	}
	static inline bool IsRookPromotionWithCapture(Move move) {
		return (move & FLAG_MASK) == CAPTURE_PROMOTION_ROOK_MASK;
	}
	static inline bool IsQueenPromotionWithCapture(Move move) {
		return (move & FLAG_MASK) == CAPTURE_PROMOTION_QUEEN_MASK;
	}

	static Square GetTo(Move move) {
		return (move & TO_MASK) >> TO_SHIFT;
	}
	static Square GetFrom(Move move) {
		return (move & FROM_MASK) >> FROM_SHIFT;
	}
	static unsigned __int8 GetFlag(Move move) {
		return (move & FLAG_MASK) >> FLAG_SHIFT;
	}

	static Move Create(unsigned __int64 from, unsigned __int64 to, unsigned __int64 flags) {
		return (flags << FLAG_SHIFT) | (to << TO_SHIFT) | (from << FROM_SHIFT);
	}


};