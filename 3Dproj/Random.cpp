#include "Random.h"

float RandomNumber(float min, float max)
{
    if (min == max && min == 0) {
        return 0;
    }
    return ((float)(rand() % (int)((max - min) * 100) + (int)((min) * 100))) / 100;
}

int RandomNumber(int min, int max)
{
    return (rand() % (max - min)) + min;
}
