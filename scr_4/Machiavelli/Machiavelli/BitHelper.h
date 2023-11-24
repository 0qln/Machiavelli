#pragma once

#include "Misc.h"

class BitHelper
{
public:
    static void DeactivateBit(Bitboard* board, const Square* squareIdx);
    static void ActivateBit(Bitboard* board, const Square* squareIdx);
    static void FlipBit(Bitboard* board, const Square* squareIdx);
    static void FlipBits(Bitboard* board, const Square* squareIdx1, const Square* squareIdx2);
    static bool GetBit(Bitboard board, const Square* squareIdx);
    static void SetBit(Bitboard* board, const Square* squareIdx, const bool value);
    static int CountBits(Bitboard bb);
    static Bitboard FromIndex(Square idx);
    static Bitboard Rotate90DegClockwise(Bitboard x);
    static Bitboard Rotate90DegAntiClockwise(Bitboard x);
    static Bitboard FlipA8H1(Bitboard x);
    static Bitboard FlipA1H8(Bitboard x);
    static Bitboard FlipVertical(Bitboard x);
    static Bitboard FlipHorizontal(Bitboard x);
    static Square LsbIdx(Bitboard board);
    static Square PopLsbIdx(Bitboard* board);
    static Square MsbIdx(Bitboard board);
    static void PrintBitboard(Bitboard board);
    static std::string ReplaceAll(std::string str, const std::string& from, const std::string& to);
};

