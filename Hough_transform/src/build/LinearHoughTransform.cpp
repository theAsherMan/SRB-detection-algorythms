#include "LinearHoughTransform.hpp"

void plotInHoughSpace(int x, int y, long value, TwoDimentionalSpace*, TwoDimentionalSpace*);

using namespace std;

TwoDimentionalSpace* ImageSpaceToHoughSpace(int outputWidth, int outputHight, TwoDimentionalSpace* imageSpace)
{
    TwoDimentionalSpace* houghSpace = new TwoDimentionalSpace(outputWidth, outputHight);

    for(TwoDimentionalSpaceLocation point : *imageSpace)
    {
        plotInHoughSpace(point.x, point.y, point.value, imageSpace, houghSpace);
    }
}

void plotInHoughSpace(int x, int y, long value, TwoDimentionalSpace* imageSpace, TwoDimentionalSpace* houghSpace)
{
    if(value != 0)
    {
        for(int m=0; m<houghSpace->getWidth(); m++)
        {
            int c = -m*x + y;
            houghSpace->set(m,c, houghSpace->get(m,c)+1);
        }
    }
}