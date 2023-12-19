#pragma once

#include <vector>

#include "Misc.h"
#include "Board.h"



namespace Machiavelli {

	class Board;


	class MoveGen {

	private:
		Board* _board;



	public:
		MoveGen(Board* b);
		~MoveGen();

		int Perft(int dpeth, bool pv);

		std::vector<Move> GeneratePseudoLegalMoves();
		std::vector<Move> GeneratePseudoLegalMoves(Color c);


		Bitboard GenerateAttacks(const Square idx, Color us);

		void GeneratePseudoLegalPawnMoves(const Square idx, Color us, std::vector<Move>* movelist);
		Bitboard GeneratePawnAttacks(const Square idx, Color us);

		void GeneratePseudoLegalKnightMoves(const Square idx, Color us, std::vector<Move>* movelist);
		Bitboard GenerateKnightAttacks(const Square idx, Color us);
		
		void GeneratePseudoLegalBishopMoves(const Square idx, Color us, std::vector<Move>* movelist);
		Bitboard GeneratePseudoLegalBishopAttacks(const Square idx, Color us);
		Bitboard GenerateBishopAttacks(const Square idx, Color us);
		
		void GeneratePseudoLegalRookMoves(const Square idx, Color us, std::vector<Move>* movelist);
		Bitboard GeneratePseudoLegalRookAttacks(const Square idx, Color us);
		Bitboard GenerateRookAttacks(const Square idx, Color us);
		
		void GeneratePseudoLegalQueenMoves(const Square idx, Color us, std::vector<Move>* movelist);
		Bitboard GenerateQueenAttacks(const Square idx, Color us);
		
		void GeneratePseudoLegalKingMoves(const Square idx, Color us, std::vector<Move>* movelist);
		Bitboard GenerateKingAttacks(const Square idx, Color us);


		std::vector<Move> GenerateLegalMoves();



		const Bitboard FileMask[8]{
			0x0101010101010101ULL << 0,
			0x0101010101010101ULL << 1,
			0x0101010101010101ULL << 2,
			0x0101010101010101ULL << 3,
			0x0101010101010101ULL << 4,
			0x0101010101010101ULL << 5,
			0x0101010101010101ULL << 6,
			0x0101010101010101ULL << 7,
		};


		const Bitboard RankMask[8]{
			0b11111111ULL << 8 * 0,
			0b11111111ULL << 8 * 1,
			0b11111111ULL << 8 * 2,
			0b11111111ULL << 8 * 3,
			0b11111111ULL << 8 * 4,
			0b11111111ULL << 8 * 5,
			0b11111111ULL << 8 * 6,
			0b11111111ULL << 8 * 7,
		};


		/// <summary>
		/// Diagonals parallel to (A1 to H8)
		/// </summary>
		const Bitboard DiagMask1[15]{
			0x8040201008040201ULL / 128 & (RankMask[7]),
			0x8040201008040201ULL / 64 & (RankMask[6] | RankMask[7]),
			0x8040201008040201ULL / 32 & (RankMask[5] | RankMask[6] | RankMask[7]),
			0x8040201008040201ULL / 16 & (RankMask[4] | RankMask[5] | RankMask[6] | RankMask[7]),
			0x8040201008040201ULL / 8 & (RankMask[3] | RankMask[4] | RankMask[5] | RankMask[6] | RankMask[7]),
			0x8040201008040201ULL / 4 & (RankMask[2] | RankMask[3] | RankMask[4] | RankMask[5] | RankMask[6] | RankMask[7]),
			0x8040201008040201ULL / 2 & (RankMask[1] | RankMask[2] | RankMask[3] | RankMask[4] | RankMask[5] | RankMask[6] | RankMask[7]),
			0x8040201008040201ULL,
			0x8040201008040201ULL * 2 & (FileMask[1] | FileMask[2] | FileMask[3] | FileMask[4] | FileMask[5] | FileMask[6] | FileMask[7]),
			0x8040201008040201ULL * 4 & (FileMask[2] | FileMask[3] | FileMask[4] | FileMask[5] | FileMask[6] | FileMask[7]),
			0x8040201008040201ULL * 8 & (FileMask[3] | FileMask[4] | FileMask[5] | FileMask[6] | FileMask[7]),
			0x8040201008040201ULL * 16 & (FileMask[4] | FileMask[5] | FileMask[6] | FileMask[7]),
			0x8040201008040201ULL * 32 & (FileMask[5] | FileMask[6] | FileMask[7]),
			0x8040201008040201ULL * 64 & (FileMask[6] | FileMask[7]),
			0x8040201008040201ULL * 128 & (FileMask[7]),
		};


		/// <summary>
		/// Diagonals parallel to (A8 to H1)
		/// </summary>
		const Bitboard DiagMask2[15]{
			0x0102040810204080ULL / 128 & (FileMask[0]),
			0x0102040810204080ULL / 64 & (FileMask[1] | FileMask[0]),
			0x0102040810204080ULL / 32 & (FileMask[2] | FileMask[1] | FileMask[0]),
			0x0102040810204080ULL / 16 & (FileMask[3] | FileMask[2] | FileMask[1] | FileMask[0]),
			0x0102040810204080ULL / 8 & (FileMask[4] | FileMask[3] | FileMask[2] | FileMask[1] | FileMask[0]),
			0x0102040810204080ULL / 4 & (FileMask[5] | FileMask[4] | FileMask[3] | FileMask[2] | FileMask[1] | FileMask[0]),
			0x0102040810204080ULL / 2 & (FileMask[6] | FileMask[5] | FileMask[4] | FileMask[3] | FileMask[2] | FileMask[1] | FileMask[0]),
			0x0102040810204080ULL,
			0x0102040810204080ULL * 2 & (FileMask[7] | FileMask[6] | FileMask[5] | FileMask[4] | FileMask[3] | FileMask[2] | FileMask[1]),
			0x0102040810204080ULL * 4 & (FileMask[7] | FileMask[6] | FileMask[5] | FileMask[4] | FileMask[3] | FileMask[2]),
			0x0102040810204080ULL * 8 & (FileMask[7] | FileMask[6] | FileMask[5] | FileMask[4] | FileMask[3]),
			0x0102040810204080ULL * 16 & (FileMask[7] | FileMask[6] | FileMask[5] | FileMask[4]),
			0x0102040810204080ULL * 32 & (FileMask[7] | FileMask[6] | FileMask[5]),
			0x0102040810204080ULL * 64 & (FileMask[7] | FileMask[6]),
			0x0102040810204080ULL * 128 & (FileMask[7]),
		};
	};
}


