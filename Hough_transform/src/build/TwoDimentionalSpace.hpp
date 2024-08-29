#pragma once

#include <iostream>

class TwoDimentionalSpace;
class ITwoDimentionalSpace;
class TwoDimantionalSpaceLocation;

struct TwoDimentionalSpaceLocation
{
    int x;
    int y;
    long value;
};

class ITwoDimentionalSpace
{
private:
    int width;
    int height;

    int x;
    int y;

    TwoDimentionalSpace* space;

public:
    ITwoDimentionalSpace(int, int, TwoDimentionalSpace*);

    TwoDimentionalSpaceLocation operator*();

    void operator++();

    bool operator!=(ITwoDimentionalSpace);

    bool operator==(ITwoDimentionalSpace);
};

class TwoDimentionalSpace
{
private:
    int width;
    int height;
    long** array;
public:
    TwoDimentionalSpace(int width, int height);
    ~TwoDimentionalSpace();

    long get(int, int);
    void set(int, int, long);

    int getWidth();
    int getHieght();

    ITwoDimentionalSpace begin();
    ITwoDimentionalSpace end();

    void print();
};

