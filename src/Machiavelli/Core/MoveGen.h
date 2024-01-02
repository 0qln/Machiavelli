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
		static const Bitboard FileMask[8];
		static const Bitboard RankMask[8];
		static const Bitboard DiagMask1[15];
		static const Bitboard DiagMask2[15];


		MoveGen(Board* b);
		~MoveGen();

		int Perft(int dpeth, bool pv);

		std::vector<Move> GeneratePseudoLegalMoves();
		std::vector<Move> GeneratePseudoLegalMoves(Color c);


		std::vector<Move> GenerateLegalMoves();
		std::vector<Move> GenerateLegalMoves(Color c);

		void GenerateLegalMoves_Check(Color c, std::vector<Move>* movelist);
		void GenerateLegalMoves(Color c, std::vector<Move>* movelist);

		Bitboard GenerateAttacks(const Square idx, Color us);

		void GenerateLegalPawnMoves(const Square idx, Color us, std::vector<Move>* movelist);
		void GeneratePseudoLegalPawnMoves(const Square idx, Color us, std::vector<Move>* movelist);
		void GenerateLegalPawnMoves_Check(const Square idx, Color us, std::vector<Move>* movelist);
		Bitboard GeneratePawnAttacks(const Square idx, Color us);

		void GeneratePseudoLegalKnightMoves(const Square idx, Color us, std::vector<Move>* movelist);
		void GenerateLegalKnightMoves_Check(const Square idx, Color us, std::vector<Move>* movelist);
		Bitboard GenerateKnightAttacks(const Square idx, Color us);
		
		void GeneratePseudoLegalBishopMoves(const Square idx, Color us, std::vector<Move>* movelist);
		void GenerateLegalBishopMoves_Check(const Square idx, Color us, std::vector<Move>* movelist);
		Bitboard GeneratePseudoLegalBishopAttacks(const Square idx, Color us);
		template <CompassRose Direction>
		Bitboard GenerateBishopPin(const Square bishopIdx, const Square kingIdx, Color us);
		Bitboard GenerateBishopAttacks(const Square idx, Color us);
		
		void GeneratePseudoLegalRookMoves(const Square idx, Color us, std::vector<Move>* movelist);
		void GenerateLegalRookMoves_Check(const Square idx, Color us, std::vector<Move>* movelist);
		Bitboard GeneratePseudoLegalRookAttacks(const Square idx, Color us);
		Bitboard GenerateRookAttacks(const Square idx, Color us);
		Bitboard GenerateRookRaw(const Square idx);

		void GeneratePseudoLegalQueenMoves(const Square idx, Color us, std::vector<Move>* movelist);
		void GenerateLegalQueenMoves_Check(const Square idx, Color us, std::vector<Move>* movelist);
		Bitboard GenerateQueenAttacks(const Square idx, Color us);
		
		void GeneratePseudoLegalKingMoves(const Square idx, Color us, std::vector<Move>* movelist);
		void GenerateLegalKingMoves_Check(const Square idx, Color us, std::vector<Move>* movelist);
		void GenerateLegalKingMoves(const Square idx, Color us, std::vector<Move>* movelist);
		Bitboard GenerateKingAttacks(const Square idx, Color us);
	};
}


