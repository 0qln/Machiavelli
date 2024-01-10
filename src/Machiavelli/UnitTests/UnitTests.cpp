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
            Assert::AreEqual(197281, gen1.Perft<false>(4, printInfo));
        }

        TEST_METHOD(Position2)
        {
            auto b2 = Machiavelli::Board::FromFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
            auto gen2 = Machiavelli::MoveGen::MoveGen(&b2);
            Assert::AreEqual(4085603, gen2.Perft<false>(4, printInfo));
        }

        TEST_METHOD(Position3)
        {
            auto b3 = Machiavelli::Board::FromFEN("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ");
            auto gen3 = Machiavelli::MoveGen::MoveGen(&b3);
            Assert::AreEqual(43238, gen3.Perft<false>(4, printInfo));
        }

        TEST_METHOD(Position4)
        {
            auto b4 = Machiavelli::Board::FromFEN("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
            auto gen4 = Machiavelli::MoveGen::MoveGen(&b4);
            Assert::AreEqual(422333, gen4.Perft<false>(4, printInfo));
        }

        TEST_METHOD(Position5)
        {
            auto b5 = Machiavelli::Board::FromFEN("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
            auto gen5 = Machiavelli::MoveGen::MoveGen(&b5);
            Assert::AreEqual(2103487, gen5.Perft<false>(4, printInfo));
        }

        TEST_METHOD(Position6)
        {
            auto b6 = Machiavelli::Board::FromFEN("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
            auto gen6 = Machiavelli::MoveGen::MoveGen(&b6);
            Assert::AreEqual(3894594, gen6.Perft<false>(4, printInfo));
        }
    };


    TEST_CLASS(Pins) {
        void RunRookTest(const std::string& fen, Machiavelli::Bitboard expectedPins, Machiavelli::Color toTestColor) {
            RunTest(fen, expectedPins, Machiavelli::PieceType::Rook, toTestColor);
        }

        void RunBishopTest(const std::string& fen, Machiavelli::Bitboard expectedPins, Machiavelli::Color toTestColor) {
            RunTest(fen, expectedPins, Machiavelli::PieceType::Bishop, toTestColor);
        }

        void RunQueenTest(const std::string& fen, Machiavelli::Bitboard expectedPins, Machiavelli::Color toTestColor) {
            RunTest(fen, expectedPins, Machiavelli::PieceType::Queen, toTestColor);
        }

        void RunTest(const std::string& fen, Machiavelli::Bitboard expectedPins, Machiavelli::PieceType pieceType, Machiavelli::Color toTestColor) {
            Machiavelli::Board b;
            b.SetUpFEN(fen);
            Assert::AreEqual(expectedPins, b.GetPinnedPieces(toTestColor));
        }

        TEST_METHOD(RookTests) {
            // enemy pins
            RunRookTest("8/8/8/8/1R2bk2/8/8/4K3 w - - 0 1", 268435456ULL, Machiavelli::Color::Black);
            RunRookTest("8/8/4k3/8/4b3/8/4R3/K7 w - - 0 1", 268435456ULL, Machiavelli::Color::Black);
            RunRookTest("8/8/8/2k2b1R/8/8/8/K7 w - - 0 1", 137438953472ULL, Machiavelli::Color::Black);
            RunRookTest("8/6R1/8/8/6b1/8/8/K5k1 w - - 0 1", 1073741824ULL, Machiavelli::Color::Black);
            // ally pins
            RunRookTest("8/8/8/8/1R2bk2/8/8/4K3 b - - 0 1", 268435456ULL, Machiavelli::Color::Black);
            RunRookTest("8/8/4k3/8/4b3/8/4R3/K7 b - - 0 1", 268435456ULL, Machiavelli::Color::Black);
            RunRookTest("8/8/8/2k2b1R/8/8/8/K7 b - - 0 1", 137438953472ULL, Machiavelli::Color::Black);
            RunRookTest("8/6R1/8/8/6b1/8/8/K5k1 b - - 0 1", 1073741824ULL, Machiavelli::Color::Black);
        }

        TEST_METHOD(BishopTests) {
            // enemy pins
            RunBishopTest("8/3B4/8/5r2/8/7k/8/K7 w - - 0 1", 137438953472ULL, Machiavelli::Color::Black);
            RunBishopTest("8/4B3/8/2r5/8/k7/8/K7 w - - 0 1", 17179869184ULL, Machiavelli::Color::Black);
            RunBishopTest("8/8/6k1/5r2/8/3B4/8/3K4 w - - 0 1", 137438953472ULL, Machiavelli::Color::Black);
            RunBishopTest("k7/8/8/3r4/8/5B2/8/4K3 w - - 0 1", 34359738368ULL, Machiavelli::Color::Black);
            // ally pins
            RunBishopTest("8/3B4/8/5r2/8/7k/8/K7 b - - 0 1", 137438953472ULL, Machiavelli::Color::Black);
            RunBishopTest("8/4B3/8/2r5/8/k7/8/K7 b - - 0 1", 17179869184ULL, Machiavelli::Color::Black);
            RunBishopTest("8/8/6k1/5r2/8/3B4/8/3K4 b - - 0 1", 137438953472ULL, Machiavelli::Color::Black);
            RunBishopTest("k7/8/8/3r4/8/5B2/8/4K3 b - - 0 1", 34359738368ULL, Machiavelli::Color::Black);
        }

        TEST_METHOD(QueenTests) {
            // enemy pins
            RunQueenTest("8/8/8/8/1Q2bk2/8/8/4K3 w - - 0 1", 268435456ULL, Machiavelli::Color::Black);
            RunQueenTest("8/8/4k3/8/4b3/8/4Q3/K7 w - - 0 1", 268435456ULL, Machiavelli::Color::Black);
            RunQueenTest("8/8/8/2k2b1Q/8/8/8/K7 w - - 0 1", 137438953472ULL, Machiavelli::Color::Black);
            RunQueenTest("8/6Q1/8/8/6b1/8/8/K5k1 w - - 0 1", 1073741824ULL, Machiavelli::Color::Black);
            RunQueenTest("8/3Q4/8/5r2/8/7k/8/K7 w - - 0 1", 137438953472ULL, Machiavelli::Color::Black);
            RunQueenTest("8/4Q3/8/2r5/8/k7/8/K7 w - - 0 1", 17179869184ULL, Machiavelli::Color::Black);
            RunQueenTest("8/8/6k1/5r2/8/3Q4/8/3K4 w - - 0 1", 137438953472ULL, Machiavelli::Color::Black);
            RunQueenTest("k7/8/8/3r4/8/5Q2/8/4K3 w - - 0 1", 34359738368ULL, Machiavelli::Color::Black);
            // ally pins
            RunQueenTest("8/8/8/8/1Q2bk2/8/8/4K3 b - - 0 1", 268435456ULL, Machiavelli::Color::Black);
            RunQueenTest("8/8/4k3/8/4b3/8/4Q3/K7 b - - 0 1", 268435456ULL, Machiavelli::Color::Black);
            RunQueenTest("8/8/8/2k2b1Q/8/8/8/K7 b - - 0 1", 137438953472ULL, Machiavelli::Color::Black);
            RunQueenTest("8/6Q1/8/8/6b1/8/8/K5k1 b - - 0 1", 1073741824ULL, Machiavelli::Color::Black);
            RunQueenTest("8/3Q4/8/5r2/8/7k/8/K7 b - - 0 1", 137438953472ULL, Machiavelli::Color::Black);
            RunQueenTest("8/4Q3/8/2r5/8/k7/8/K7 b - - 0 1", 17179869184ULL, Machiavelli::Color::Black);
            RunQueenTest("8/8/6k1/5r2/8/3Q4/8/3K4 b - - 0 1", 137438953472ULL, Machiavelli::Color::Black);
            RunQueenTest("k7/8/8/3r4/8/5Q2/8/4K3 b - - 0 1", 34359738368ULL, Machiavelli::Color::Black);
        }
    };

}
