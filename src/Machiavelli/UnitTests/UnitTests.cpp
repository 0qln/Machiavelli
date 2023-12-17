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
            auto b1 = Machiavelli::Board::Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
            auto gen1 = Machiavelli::MoveGen::MoveGen(&b1);
            Assert::AreEqual(197281, gen1.Perft(4, printInfo));
        }

        TEST_METHOD(Position2)
        {
            auto b2 = Machiavelli::Board::Board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
            auto gen2 = Machiavelli::MoveGen::MoveGen(&b2);
            Assert::AreEqual(4085603, gen2.Perft(4, printInfo));
        }

        TEST_METHOD(Position3)
        {
            auto b3 = Machiavelli::Board::Board("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ");
            auto gen3 = Machiavelli::MoveGen::MoveGen(&b3);
            Assert::AreEqual(43238, gen3.Perft(4, printInfo));
        }

        TEST_METHOD(Position4)
        {
            auto b4 = Machiavelli::Board::Board("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
            auto gen4 = Machiavelli::MoveGen::MoveGen(&b4);
            Assert::AreEqual(422333, gen4.Perft(4, printInfo));
        }

        TEST_METHOD(Position5)
        {
            auto b5 = Machiavelli::Board::Board("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
            auto gen5 = Machiavelli::MoveGen::MoveGen(&b5);
            Assert::AreEqual(2103487, gen5.Perft(4, printInfo));
        }

        TEST_METHOD(Position6)
        {
            auto b6 = Machiavelli::Board::Board("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
            auto gen6 = Machiavelli::MoveGen::MoveGen(&b6);
            Assert::AreEqual(3894594, gen6.Perft(4, printInfo));
        }
    };
}
