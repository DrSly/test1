#ifndef RANDOM_H
#define RANDOM_H

#include <QTime>

class QTime;

class Random
{
public:
    Random();
    ~Random();
    static int getRandomNumber(int low, int high);
private:
    static Random *s_instance;
    static Random *getInstance();
    int getRandomReal(int low, int high);
};

#endif // RANDOM_H
