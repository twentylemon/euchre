#include "Random.h"


Random::Random() {
}


/**
 * @return a random boolean
 */
bool Random::nextBool() {
    return nextInt(2) == 0;
}

/**
 * @return a random int in the range 0..getMax()
 */
unsigned int Random::nextInt() {
    return std::rand();
}

/**
 * @param n the modulus
 * @return a random int mod n
 */
unsigned int Random::nextInt(unsigned int n) {
    return nextInt() % n;
}

/**
 * @return a random float in [0..1)
 */
float Random::nextFloat() {
    return (float)nextInt() / (float)getRandMax();
}

/**
 * @return a random double in [0..1)
 */
double Random::nextDouble() {
    return (double)nextInt() / (double)getRandMax();
}

/**
 * @return the max random value, exclusive
 */
unsigned int Random::getRandMax() {
    return RAND_MAX;
}

/**
 * @param seed the new seed to set
 */
void Random::setSeed(unsigned int seed) {
    std::srand(seed);
}

/**
 * inits the seed with the current time
 */
void Random::initSeed() {
    setSeed((unsigned)std::time(nullptr));
}
