#include "VisualSpace.hpp"
#include <iostream>

VisualSpace::VisualSpace(int width, int height)
{
    this->array = new vector<vector<VSPoint*>*>();
    for(int ii=0; ii<width; ii++)
    {
        this->array->push_back(new vector<VSPoint*>());
        for(int jj=0; jj<height; jj++)
        {
            this->array->back()->push_back(new VSPoint(ii, jj, 0));
        }
    }

    this->width = width;
    this->hieght = height;
}

VisualSpace::~VisualSpace()
{
    for(int ii=0; ii<this->width; ii++)
    {
        for(int jj=0; jj<this->hieght; jj++)
        {
            delete (this->array->at(ii)->at(jj));
        }
        delete (this->array->at(ii));
    }
    delete (this->array);
}

int VisualSpace::getWidth()
{
    return this->width;
}

int VisualSpace::getHieght()
{
    return this->hieght;
}

VSPoint* VisualSpace::point(int x, int y)
{
    return this->array->at(x)->at(y);
}

string VisualSpace::toString()
{
    string result = "";
    for(int y=this->getHieght()-1; y>= 0; y--)
    {
        for(int x=0; x<this->getWidth(); x++)
        {
            result += " : ";
            result += to_string(this->point(x,y)->getValue());
        }
        result += " :";
        result += "\n";
    }
    return result;
}

// -------------------------------------------------------------------

VSPoint::VSPoint(int x, int y, double value)
{
    this->x = x;
    this->y = y;
    this->value = value;
}
int VSPoint::getX()
{
    return x;
}
int VSPoint::getY()
{
    return y;
}
double VSPoint::getValue()
{
    return value;
}
double VSPoint::increaseValue(double value)
{
    this->value += value;
}