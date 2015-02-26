
#pragma once
#include <ctime>
#include <cstdlib>

class Random
{
public:
    static bool nextBool();
    static unsigned int nextInt();
    static unsigned int nextInt(unsigned int n);

    static float nextFloat();
    static double nextDouble();

    static unsigned int getRandMax();

    static void setSeed(unsigned int seed);
    static void initSeed();

private:
    Random();
};
