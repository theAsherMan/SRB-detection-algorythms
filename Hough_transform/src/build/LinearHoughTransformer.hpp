#pragma once

#define _USE_MATH_DEFINES

using namespace std;

#include <math.h>
#include <vector>
#include <string>

#include "VisualSpace.hpp"

template <typename T>
class tuple2{
public:
    T x;
    T y;
};

class HoughLineDescriptor
{
public:
    
    double theta;
    double rho;
    double brightness;
    HoughLineDescriptor(double theta, double rho, double brightness)
    {
        this->theta = theta;
        this->rho = rho;
        this->brightness = brightness;
    }
    string toString()
    {
        return "xCos(" + to_string(theta) + ") + ySin(" + to_string(theta) + ") = " + to_string(rho) + ": brightness= " + to_string(brightness);
    }
};
#define DUMMY_LINE_DESCRIPTOR_MAX HoughLineDescriptor(0,0,INFINITY)
#define DUMMY_LINE_DESCRIPTOR_MIN HoughLineDescriptor(0,0,-INFINITY)

class IndexThetaMaping
{
public:
    IndexThetaMaping(int index, double delta_slope, double min_slope)
    {
        this->_index = index;
        this->_theta = index*delta_slope + min_slope;
    }
    IndexThetaMaping(double theta, double delta_slope, double min_slope)
    {
        this->_theta = theta;
        this->_index = round((theta-min_slope)/delta_slope);
    }
    int index(){return _index;}
    double theta(){return _theta;}

private:
    double _theta;
    int _index;
};
class IndexThetaMappingFactory
{
private:
    double delta_slope;
    double min_slope;
public:
    IndexThetaMappingFactory(double delta_slope, double min_slope)
    {
        this->delta_slope = delta_slope;
        this->min_slope = min_slope;
    }
    IndexThetaMaping mapFromIndex(int index)
    {
        return IndexThetaMaping(index, delta_slope, min_slope);
    }
    IndexThetaMaping mapFromTheta(double theta)
    {
        return IndexThetaMaping(theta, delta_slope, min_slope);
    }
};
class LinearHoughTransformer{
private:
    VisualSpace* linearSpace;
    VisualSpace* houghSpace;
    double threshold;
    IndexThetaMappingFactory* indexToTheta;

    int min_slope;
    int max_slope;
    double delta_slope;
    vector<IndexThetaMaping> thetas;


    vector<HoughLineDescriptor> linesByBrightness;

    void addNextBrightestLine();
    double getHighestBrightness();
    void transform();
    void setHoughSpace(int, int);
    void plotPointInHoughSpace(int, int);
    void VoteForLineOfSlopePassingThroughPoint(IndexThetaMaping, VSPoint*);
    void addLineToHoughSpace(IndexThetaMaping, double, double);
    HoughLineDescriptor sampleFromHoughSpace(IndexThetaMaping, double);
    HoughLineDescriptor sampleFromHoughSpace(int, int);
public:
    LinearHoughTransformer(double, double, int, double, VisualSpace*);
    void setThreshold(double threshold);
    vector<HoughLineDescriptor> getNBrightestLines(int);
    vector<HoughLineDescriptor> getLinesAboveNBrighness(double);
    vector<HoughLineDescriptor> getLinesOfHighestBrightness(double);
    HoughLineDescriptor getLine(double, int);
    string showHoughSpace();
    vector<IndexThetaMaping>getThetas();
    ~LinearHoughTransformer();
};