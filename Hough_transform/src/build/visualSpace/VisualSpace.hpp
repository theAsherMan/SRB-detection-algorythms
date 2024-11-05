#pragma once

#include <vector>
#include <string>

using namespace std;

class VSPoint{
    private:
        double value;
        int x;
        int y;
    public:
        VSPoint(int, int, double);
        int getX();
        int getY();
        double getValue();
        double increaseValue(double);
};

class VisualSpace{
    private:
        vector<vector<VSPoint*>*>* array;
        int width;
        int hieght;
    public:
        VisualSpace(int, int);
        ~VisualSpace();
        VSPoint* point(int, int);
        int getWidth();
        int getHieght();
        double getImageDiamiter();
        string toString();
        string toString(string);
};