#include "pch.h"
#include "BitHelper.h"
#include "Misc.h"

#include <iostream>
#include <bitset>

namespace Machiavelli
{

    void Machiavelli::BitHelper::DeactivateBit(Bitboard* board, const Square* squareIdx) {
        *board &= ~(1ULL << *squareIdx);
    }

    void Machiavelli::BitHelper::ActivateBit(Bitboard* board, const Square* squareIdx) {
        *board |= (1ULL << *squareIdx);
    }

    void Machiavelli::BitHelper::FlipBit(Bitboard* board, const Square* squareIdx) {
        *board ^= (1ULL << *squareIdx);
    }

    void Machiavelli::BitHelper::FlipBits(Bitboard* board, const Square* squareIdx1, const Square* squareIdx2) {
        *board ^= (1ULL << *squareIdx1 | 1ULL << *squareIdx2);
    }

    bool Machiavelli::BitHelper::GetBit(Bitboard board, const Square* squareIdx) {
        return (board >> *squareIdx) & 1;
    }

    void Machiavelli::BitHelper::SetBit(Bitboard* board, const Square* squareIdx, const bool value) {
        *board = (*board & ~(1ULL << *squareIdx)) | (static_cast<Bitboard>(value) << *squareIdx);
    }

    int Machiavelli::BitHelper::CountBits(Bitboard bb) {
        int result = 0;
        while (PopLsbIdx(&bb) != 64) {
            ++result;
        }
        return result;
    }

    Bitboard Machiavelli::BitHelper::FromIndex(Square idx) {
        return 0xFFFFFFFFFFFFFFFFULL << idx;
    }

    Bitboard Machiavelli::BitHelper::Rotate90DegClockwise(Bitboard x) {
        return FlipVertical(FlipA1H8(x));
    }

    Bitboard Machiavelli::BitHelper::Rotate90DegAntiClockwise(Bitboard x) {
        return FlipA8H1(FlipVertical(x));
    }

    Bitboard Machiavelli::BitHelper::FlipA8H1(Bitboard x) {
        Bitboard t;
        const Bitboard k1 = 0x5500550055005500ULL;
        const Bitboard k2 = 0x3333000033330000ULL;
        const Bitboard k4 = 0x0f0f0f0f00000000ULL;
        t = k4 & (x ^ (x << 28));
        x ^= t ^ (t >> 28);
        t = k2 & (x ^ (x << 14));
        x ^= t ^ (t >> 14);
        t = k1 & (x ^ (x << 7));
        x ^= t ^ (t >> 7);
        return x;
    }

    Bitboard Machiavelli::BitHelper::FlipA1H8(Bitboard x) {
        Bitboard t;
        const Bitboard k1 = 0x5500550055005500ULL;
        const Bitboard k2 = 0x3333000033330000ULL;
        const Bitboard k4 = 0x0f0f0f0f00000000ULL;
        t = k4 & (x ^ (x << 28));
        x ^= t ^ (t >> 28);
        t = k2 & (x ^ (x << 14));
        x ^= t ^ (t >> 14);
        t = k1 & (x ^ (x << 7));
        x ^= t ^ (t >> 7);
        return x;
    }

    Bitboard Machiavelli::BitHelper::FlipVertical(Bitboard x) {
        return std::byteswap(x);
    }

    Bitboard Machiavelli::BitHelper::FlipHorizontal(Bitboard x)
    {
        const Bitboard k1 = 0x5555555555555555ULL;
        const Bitboard k2 = 0x3333333333333333ULL;
        const Bitboard k4 = 0x0f0f0f0f0f0f0f0fULL;
        x = ((x >> 1) & k1) | ((x & k1) << 1);
        x = ((x >> 2) & k2) | ((x & k2) << 2);
        x = ((x >> 4) & k4) | ((x & k4) << 4);
        return x;
    }

    Square Machiavelli::BitHelper::LsbIdx(Bitboard board) {
        return board ? std::countr_zero(board) : 63;
    }

    Square Machiavelli::BitHelper::PopLsbIdx(Bitboard* board) {
        Square lsb = std::countr_zero(*board);
        *board &= *board - 1;
        return lsb;
    }

    Square Machiavelli::BitHelper::MsbIdx(Bitboard board) {
        return board ? std::countl_zero(board) : 63;
    }

    void Machiavelli::BitHelper::PrintBitboard(Bitboard board)
    {
        auto str = std::bitset<64>(FlipHorizontal(board)).to_string();
        for (Rank i = 0; i <= 7; i++) {
            std::cout << ReplaceAll(ReplaceAll(str.substr(i * 8, 8), "0", "0 "), "1", "1 ") << '\n';
        }
        std::cout << '\n';
    }

    std::string Machiavelli::BitHelper::ReplaceAll(std::string str, const std::string& from, const std::string& to) {
        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
        }
        return str;
    }
}