#include "LinearHoughTransformer.hpp"

using namespace std;

//class LinearHoughTransformer{
//    void addAreaToTransform(VisualSpace* area);
//    void resetTransformArea();
//    void setThreshold(double threshold);
//    vector<Line> getNclearestLinesAboveThreshold(int n);
//    vector<Line> getAllLinesAboveThreshold();
//};

LinearHoughTransformer::LinearHoughTransformer(int bucketsPerRadian, int imageDiamiter)
{
    this->houghSpace = new VisualSpace(ceil(M_PI*bucketsPerRadian), imageDiamiter);
    this->threshold = 0;
}

void LinearHoughTransformer::plotInHoughSpace(int ii, int jj)
{
    if(houghSpace->point(ii, jj)->getValue() >= threshold)
    {
        for(int theta=0; theta<houghSpace->getWidth(); theta++)
        {
            int rho = 
        }
    }
}

void LinearHoughTransformer::addAreaToTransform(VisualSpace* area)
{
    for(int ii=0; ii<area->getWidth(); ii++)
    {
        for(int jj=0; jj<area->getHieght(); jj++)
        {
            plotInHoughSpace(ii, jj);
        }
    }
}

VisualSpace* imageSpaceToHoughSpace(VisualSpace* imageSpace)
{
    int max_radius_of_image = sqrt(pow(imageSpace->getHieght(),2) + pow(imageSpace->getWidth(),2));
    int precision = (imageSpace->getWidth() + imageSpace->getHieght())/2;
    
    VisualSpace* houghSpace = new VisualSpace(M_PI*precision, max_radius_of_image);

    for(int y=0; y<imageSpace->getHieght(); y++)
    {
        for(int x=0; x<imageSpace->getWidth(); x++)
        {
            plotInHoughSpace(x,y,imageSpace->point(x,y), houghSpace);
        }
    }

    return houghSpace;
}

void plotInHoughSpace(int x, int y, VSPoint* point, VisualSpace* houghSpace)
{
    if(point->getValue() > 0)
    {
        for(int m=0; m<houghSpace->getWidth(); m++)
        {
            int c = -m*x + y;
            if(c < houghSpace->getHieght() && c >= 0)
            {
                houghSpace->point(m,c)->increaseValue(point->getValue());
            }
        }
    }
}