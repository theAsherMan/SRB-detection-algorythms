#include "LinearHoughTransform.hpp"
#include <vector>

using namespace std;

void plotInHoughSpace(int, int, long, vector<vector<long>*>*);
vector<vector<long>*>* allocateHoughSpace(int, int);

vector<vector<long>*>* imageSpaceToHoughSpace(vector<vector<long>*>* imageSpace)
{
    int height = imageSpace->size();
    int width = imageSpace->at(0)->size();
    vector<vector<long>*>* houghSpace = allocateHoughSpace(width, height);

    for(int y=0; y<height; y++)
    {
        for(int x=0; x<width; x++)
        {
            plotInHoughSpace(x,y,imageSpace->at(y)->at(x), houghSpace);
        }
    }

    return houghSpace;
}

void plotInHoughSpace(int x, int y, long value, vector<vector<long>*>* houghSpace)
{
    if(value != 0)
    {
        for(int m=0; m<houghSpace->size(); m++)
        {
            int c = -m*x + y;
            if(c < houghSpace->at(0)->size() && c >= 0)
            {
                houghSpace->at(c)->at(m) += value;
            }
        }
    }
}

vector<vector<long>*>* allocateHoughSpace(int width, int height)
{
    vector<vector<long>*>* houghSpace = new vector<vector<long>*>();
    for(int ii=0; ii<height; ii++)
    {
        houghSpace->push_back(new vector<long>());
        for(int jj=0; jj<width; jj++)
        {
            houghSpace->back()->push_back(0L);
        }
    }

    return houghSpace;
}