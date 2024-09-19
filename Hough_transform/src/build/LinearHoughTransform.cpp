#include "LinearHoughTransform.hpp"

using namespace std;

void plotInHoughSpace(int, int, VSPoint*, VisualSpace*);

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