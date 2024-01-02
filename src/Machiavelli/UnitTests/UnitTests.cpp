#include "pch.h"
#include "CppUnitTest.h"

#include "../Core/Board.h"
#include "../Core/MoveGen.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace UnitTests
{
    //https://www.chessprogramming.org/Perft_Results
    TEST_CLASS(MoveGeneration)
    {
    private:
        const static bool printInfo = true;

    public:
        TEST_METHOD(Position1)
        {
            auto b1 = Machiavelli::Board::FromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
            auto gen1 = Machiavelli::MoveGen::MoveGen(&b1);
            Assert::AreEqual(197281, gen1.Perft(4, printInfo));
        }

        TEST_METHOD(Position2)
        {
            auto b2 = Machiavelli::Board::FromFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
            auto gen2 = Machiavelli::MoveGen::MoveGen(&b2);
            Assert::AreEqual(4085603, gen2.Perft(4, printInfo));
        }

        TEST_METHOD(Position3)
        {
            auto b3 = Machiavelli::Board::FromFEN("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ");
            auto gen3 = Machiavelli::MoveGen::MoveGen(&b3);
            Assert::AreEqual(43238, gen3.Perft(4, printInfo));
        }

        TEST_METHOD(Position4)
        {
            auto b4 = Machiavelli::Board::FromFEN("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
            auto gen4 = Machiavelli::MoveGen::MoveGen(&b4);
            Assert::AreEqual(422333, gen4.Perft(4, printInfo));
        }

        TEST_METHOD(Position5)
        {
            auto b5 = Machiavelli::Board::FromFEN("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
            auto gen5 = Machiavelli::MoveGen::MoveGen(&b5);
            Assert::AreEqual(2103487, gen5.Perft(4, printInfo));
        }

        TEST_METHOD(Position6)
        {
            auto b6 = Machiavelli::Board::FromFEN("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
            auto gen6 = Machiavelli::MoveGen::MoveGen(&b6);
            Assert::AreEqual(3894594, gen6.Perft(4, printInfo));
        }
    };


    TEST_CLASS(Pins) {

        // Rook
        TEST_METHOD(RookLeft) {
            Machiavelli::Board b;
            b.SetUpFEN("8/8/8/8/1R2bk2/8/8/4K3 w - - 0 1");
            Assert::AreEqual(268435456ULL, b.GetPinnedPieces(Machiavelli::Black));
        }

        TEST_METHOD(RookUnder) {
            Machiavelli::Board b;
            b.SetUpFEN("8/8/4k3/8/4b3/8/4R3/K7 w - - 0 1");
            Assert::AreEqual(268435456ULL, b.GetPinnedPieces(Machiavelli::Black));
        }

        TEST_METHOD(RookRight) {
            Machiavelli::Board b;
            b.SetUpFEN("8/8/8/2k2b1R/8/8/8/K7 w - - 0 1");
            Assert::AreEqual(137438953472ULL, b.GetPinnedPieces(Machiavelli::Black));
        }

        TEST_METHOD(RookOver) {
            Machiavelli::Board b;
            b.SetUpFEN("8/6R1/8/8/6b1/8/8/K5k1 w - - 0 1");
            Assert::AreEqual(1073741824ULL, b.GetPinnedPieces(Machiavelli::Black));
        }
        
        // Bishop
        TEST_METHOD(BishopUpLeft) {
            Machiavelli::Board b;
            b.SetUpFEN("8/3B4/8/5r2/8/7k/8/K7 w - - 0 1");
            Assert::AreEqual(137438953472ULL, b.GetPinnedPieces(Machiavelli::Black));
        }

        TEST_METHOD(BishopUpRight) {
            Machiavelli::Board b;
            b.SetUpFEN("8/4B3/8/2r5/8/k7/8/K7 w - - 0 1");
            Assert::AreEqual(17179869184ULL, b.GetPinnedPieces(Machiavelli::Black));
        }

        TEST_METHOD(BishopDownLeft) {
            Machiavelli::Board b;
            b.SetUpFEN("8/8/6k1/5r2/8/3B4/8/3K4 w - - 0 1");
            Assert::AreEqual(137438953472ULL, b.GetPinnedPieces(Machiavelli::Black));
        }

        TEST_METHOD(BishopDownRight) {
            Machiavelli::Board b;
            b.SetUpFEN("k7/8/8/3r4/8/5B2/8/4K3 w - - 0 1");
            Assert::AreEqual(34359738368ULL, b.GetPinnedPieces(Machiavelli::Black));
        }

        // Queen
        TEST_METHOD(QueenLeft) {
            Machiavelli::Board b;
            b.SetUpFEN("8/8/8/8/1Q2bk2/8/8/4K3 w - - 0 1");
            Assert::AreEqual(268435456ULL, b.GetPinnedPieces(Machiavelli::Black));
        }

        TEST_METHOD(QueenUnder) {
            Machiavelli::Board b;
            b.SetUpFEN("8/8/4k3/8/4b3/8/4Q3/K7 w - - 0 1");
            Assert::AreEqual(268435456ULL, b.GetPinnedPieces(Machiavelli::Black));
        }

        TEST_METHOD(QueenRight) {
            Machiavelli::Board b;
            b.SetUpFEN("8/8/8/2k2b1Q/8/8/8/K7 w - - 0 1");
            Assert::AreEqual(137438953472ULL, b.GetPinnedPieces(Machiavelli::Black));
        }

        TEST_METHOD(QueenOver) {
            Machiavelli::Board b;
            b.SetUpFEN("8/6Q1/8/8/6b1/8/8/K5k1 w - - 0 1");
            Assert::AreEqual(1073741824ULL, b.GetPinnedPieces(Machiavelli::Black));
        }
        TEST_METHOD(QueenUpLeft) {
            Machiavelli::Board b;
            b.SetUpFEN("8/3Q4/8/5r2/8/7k/8/K7 w - - 0 1");
            Assert::AreEqual(137438953472ULL, b.GetPinnedPieces(Machiavelli::Black));
        }

        TEST_METHOD(QueenUpRight) {
            Machiavelli::Board b;
            b.SetUpFEN("8/4Q3/8/2r5/8/k7/8/K7 w - - 0 1");
            Assert::AreEqual(17179869184ULL, b.GetPinnedPieces(Machiavelli::Black));
        }

        TEST_METHOD(QueenDownLeft) {
            Machiavelli::Board b;
            b.SetUpFEN("8/8/6k1/5r2/8/3Q4/8/3K4 w - - 0 1");
            Assert::AreEqual(137438953472ULL, b.GetPinnedPieces(Machiavelli::Black));
        }

        TEST_METHOD(QueenDownRight) {
            Machiavelli::Board b;
            b.SetUpFEN("k7/8/8/3r4/8/5Q2/8/4K3 w - - 0 1");
            Assert::AreEqual(34359738368ULL, b.GetPinnedPieces(Machiavelli::Black));
        }
    };
}
