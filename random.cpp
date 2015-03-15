#include "random.h"

// initialise static members here:
Random *Random::s_instance = 0;

Random::Random()
{
    // seed the random number generator using the current time
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
}

Random::~Random()
{
}

int Random::getRandomNumber(int low, int high)
{
    // return a random number between low and high
    //return qrand() % ((high + 1) - low) + low;
    return getInstance()->getRandomReal(low, high);
}

Random *Random::getInstance()
{
    if (!s_instance)
        s_instance = new Random();
    return s_instance;
}

int Random::getRandomReal(int low, int high)
{
    // return a random number between low and high
    return qrand() % ((high + 1) - low) + low;
}

