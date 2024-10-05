#include "LinearHoughTransformer.hpp"

#include <iostream>

using namespace std;

LinearHoughTransformer::LinearHoughTransformer(double min_slope, double max_slope, int number_of_slope_buckets, double threshold, VisualSpace* linearSpace)
{
    this->linearSpace = linearSpace;
    this->houghSpace = NULL;
    this->threshold = threshold;
    this->min_slope = min_slope;
    this->max_slope = max_slope;
    this->linesByBrightness = vector<HoughLineDescriptor>();
    
    this->thetas = vector<IndexThetaMaping>();
    double slope_range = max_slope - min_slope;
    this->delta_slope = slope_range/number_of_slope_buckets;
    for(int ii=0; ii<number_of_slope_buckets; ii++)
    {
        thetas.push_back(IndexThetaMaping(min_slope + ii * delta_slope, delta_slope, min_slope));
    }
    this->indexToTheta = new IndexThetaMappingFactory(delta_slope, min_slope);
}
LinearHoughTransformer::~LinearHoughTransformer()
{
    if(houghSpace != NULL)
    {
        delete(houghSpace);
    }
    if(indexToTheta != NULL)
    {
        delete(indexToTheta);
    }
}

vector<HoughLineDescriptor> LinearHoughTransformer::getNBrightestLines(int n)
{
    // this requires a dummy value to start to prevent the allocation of memory from outside the vector's bounds
    linesByBrightness = vector<HoughLineDescriptor>();
    n++;
    linesByBrightness.push_back(DUMMY_LINE_DESCRIPTOR_MAX);
    if(houghSpace == NULL)
    {
        transform();
    }
    for(int ii=0; ii<houghSpace->getWidth(); ii++)
    {
        for(int jj=0; jj<houghSpace->getHieght(); jj++)
        {
            auto point = houghSpace->point(ii, jj);
            int idx = linesByBrightness.size() - 1;
            while(linesByBrightness[idx].brightness < point->getValue()){idx--;}
            idx++;
            if(idx < n)
            {
                double theta = indexToTheta->mapFromIndex(ii).theta();
                linesByBrightness.insert(linesByBrightness.begin() + idx, HoughLineDescriptor(theta, jj, point->getValue()));
                if(linesByBrightness.size() > n){linesByBrightness.pop_back();}
            }
        }
    }
    linesByBrightness.erase(linesByBrightness.begin());
    return linesByBrightness;
}
vector<HoughLineDescriptor> LinearHoughTransformer::getLinesOfHighestBrightness(double margin)
{
    if(houghSpace == NULL)
    {
        transform();
    }
    double threshold = getHighestBrightness() - margin;
    return getLinesAboveNBrighness(threshold);
}
double LinearHoughTransformer::getHighestBrightness()
{
    double highestBrightness = -INFINITY;
    for(int ii=0; ii<houghSpace->getWidth(); ii++)
    {
        for(int jj=0; jj<houghSpace->getHieght(); jj++)
        {
            highestBrightness = max(highestBrightness, houghSpace->point(ii, jj)->getValue());
        }
    }
    return highestBrightness;
}
vector<HoughLineDescriptor> LinearHoughTransformer::getLinesAboveNBrighness(double threshold)
{
    linesByBrightness = vector<HoughLineDescriptor>();
    if(houghSpace == NULL)
    {
        transform();
    }
    for(int ii=0; ii<houghSpace->getWidth(); ii++)
    {
        for(int jj=0; jj<houghSpace->getHieght(); jj++)
        {
            auto point = houghSpace->point(ii, jj);
            if(point->getValue() >= threshold)
            {
                linesByBrightness.push_back(sampleFromHoughSpace(ii, jj));
            }
        }
    }
    return linesByBrightness;
}
HoughLineDescriptor LinearHoughTransformer::getLine(double theta, int rho)
{
    if(houghSpace == NULL)
    {
        transform();
    }
    return sampleFromHoughSpace(indexToTheta->mapFromTheta(theta), rho);
}
string LinearHoughTransformer::showHoughSpace()
{
    string thetasString = "";
    for (auto theta : thetas)
    {
        thetasString += to_string(theta.theta());
        thetasString += " : ";
    }
    return thetasString + "\n" + houghSpace->toString();
}
vector<IndexThetaMaping> LinearHoughTransformer::getThetas()
{
    return thetas;
}
void LinearHoughTransformer::transform()
{
    setHoughSpace(thetas.size(), ceil(linearSpace->getImageDiamiter()));

    for(int x = 0; x < linearSpace->getWidth(); x++)
    {
        for(int y = 0; y < linearSpace->getHieght(); y++)
        {
            plotPointInHoughSpace(x,y);
        }
    }
}
void LinearHoughTransformer::setHoughSpace(int width, int hieght)
{
    if(houghSpace != NULL)
    {
        delete(houghSpace);
    }
    houghSpace = new VisualSpace(width, hieght);
}
void LinearHoughTransformer::plotPointInHoughSpace(int x, int y)
{
    VSPoint* point = linearSpace->point(x,y);
    if(point->getValue() >= threshold)
    {
        for(auto indexThetaPair : thetas)
        {
            VoteForLineOfSlopePassingThroughPoint(indexThetaPair, point);
        }
    }
}
void LinearHoughTransformer::VoteForLineOfSlopePassingThroughPoint(IndexThetaMaping indexThetaPair, VSPoint* point)
{
    double theta = indexThetaPair.theta();
    double rho = round(point->getX()*cos(theta) + point->getY()*sin(theta));
    if(rho >= 0 && rho < houghSpace->getHieght())
    {
        addLineToHoughSpace(indexThetaPair, rho, point->getValue());
    }
}
void LinearHoughTransformer::addLineToHoughSpace(IndexThetaMaping indexThetaPair, double rho, double value)
{
    int x = indexThetaPair.index();
    houghSpace->point(x, rho)->increaseValue(value);
}
HoughLineDescriptor LinearHoughTransformer::sampleFromHoughSpace(IndexThetaMaping indexThetaPair, double rho)
{
    double theta = indexThetaPair.theta();
    int x = indexThetaPair.index();
    double brightness = houghSpace->point(x, rho)->getValue();
    return HoughLineDescriptor(theta, rho, brightness);
}
HoughLineDescriptor LinearHoughTransformer::sampleFromHoughSpace(int x, int y)
{
    double brightness = houghSpace->point(x, y)->getValue();
    double rho = houghSpace->point(x, y)->getY();
    double theta = indexToTheta->mapFromIndex(x).theta();
    return HoughLineDescriptor(theta, rho, brightness);
}