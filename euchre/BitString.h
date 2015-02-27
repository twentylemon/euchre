

#pragma once
#include <bitset>

/**
 * creates n bit strings with k bits on, where n < 32
 */
class BitString
{
public:
    static int first(int numBitsOn);
    static int next(int val);
    static int last(int numBitsOn, int n);
    /*
    template <std::size_t N> static std::bitset<N> first(int numBitsOn);
    template <std::size_t N> static std::bitset<N> next(std::bitset<N> val);
    template <std::size_t N> static std::bitset<N> last(int numBitsOn);
    */
};

