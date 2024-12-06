#pragma once

#include <functional>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <valarray>
#include <iostream>
#include <cmath>


using namespace std;

class VisualSpace;
class VSPoint{
    private:
        VisualSpace* space;
        double value;
        int x;
        int y;
    public:
        VSPoint(int, int, double, VisualSpace*);
        int getX();
        int getY();
        double getValue();
        void increaseValue(double);
        void increaseValueUnsafe(double value);
        void setValue(double);
        void setValueUnsafe(double value);
        void multiplyValue(double value);
        void multiplyValueUnsafe(double value);
};

class VisualSpace{
    private:
        valarray<VSPoint*> array;
        int width;
        int hieght;

        double _max;bool dirty_max;
        double _min;bool dirty_min;
        double _mean;bool dirty_mean;
        double _meanOfPositives; bool dirty_pos_mean;
        double _std_dev;bool dirty_dev;
        double _std_devOfPositives; bool dirty_pos_dev;

    public:
        VisualSpace(int width, int height);
        VisualSpace(const valarray<valarray<double>>*);
        VisualSpace(int, int, const valarray<double>*);
        ~VisualSpace();
        VSPoint* point(int, int);
        int getWidth();
        int getHieght();
        int count();
        double getImageRadius();
        string toString();
        string toString(string);

        void zeroAllNegitiveValues();
        void standardize();
        void standardizeIgnoringNonPositives();
        double max();
        double min();
        double mean();
        double meanOfPositives();
        double std_dev();
        double std_devOfPositives();
        void setAsDirty();

        void clear();

        valarray<double> raw();
};