

#include "BitString.h"


/**
 * @param numBitsOn the number of bits that should be on
 * @return the first bitstring with numBitsOn bits turned on
 */
 int BitString::first(int numBitsOn) {
     return (0xFFFFFFFF >> (32 - numBitsOn));
}

/**
 * @param val the current bitstring
 * @return the next bitstring with the same number of bits on as val
 */
int BitString::next(int val) {
     int t = (val | (val - 1)) + 1;
    return t | ((((t & -t) / (val & -val)) >> 1) - 1);
}

/**
 * @param numBitsOn the number of bits that should be on
 * @param n the length of the bitstring, at most 32
 * @return the last bitstring with numBitsOn and a length of n bits
 */
 int BitString::last(int numBitsOn, int n) {
     return ((0xFFFFFFFF >> (32 - n)) >> (n - numBitsOn)) << (n - numBitsOn);
}


/**
 * @param numBitsOn the number of bits that should be on
 * @return the first bitstring with numBitsOn bits turned on
 */
 /*
template <std::size_t N> static std::bitset<N> BitString::first(int numBitsOn) {
    std::bitset<N> bits;
    bits.set();
    return bits >> (bits.size() - numBitsOn);
}
*/
/**
 * @param val the current bitstring
 * @return the next bitstring with the same number of bits on as val
 */
 /*
template <std::size_t N> static std::bitset<N> BitString::next(std::bitset<N> val) {
    return std::bitset<N>();
}
*/
/**
 * @param numBitsOn the number of bits that should be on
 * @param n the length of the bitstring, at most 32
 * @return the last bitstring with numBitsOn and a length of n bits
 */
 /*
template <std::size_t N> static std::bitset<N> BitString::last(int numBitsOn) {
    return std::bitset<N>();
}
*/