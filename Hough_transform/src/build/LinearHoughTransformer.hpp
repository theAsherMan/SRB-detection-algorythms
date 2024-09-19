#pragma once

#define _USE_MATH_DEFINES

using namespace std;

#include <math.h>
#include <vector>

#include "VisualSpace.hpp"
#include "line.hpp"

class LinearHoughTransformer{
private:
    VisualSpace* houghSpace;
    double threshold;

    void plotInHoughSpace(int, int);
public:
    LinearHoughTransformer(int, int);
    void addAreaToTransform(VisualSpace* area);
    void resetTransformArea();
    void setThreshold(double threshold);
    vector<Line> getNclearestLinesAboveThreshold(int n);
    vector<Line> getAllLinesAboveThreshold();
    ~LinearHoughTransformer();
};