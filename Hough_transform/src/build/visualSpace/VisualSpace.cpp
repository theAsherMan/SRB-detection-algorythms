#include "VisualSpace.hpp"
#include <iostream>
#include <cmath>

using namespace std;

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

double VisualSpace::getImageDiamiter()
{
    double heightFromOrigin = double(hieght)/2;
    double widthFromOrigin = double(width)/2;

    return sqrt(pow(heightFromOrigin,2.0) + pow(widthFromOrigin,2.0));
}

VSPoint* VisualSpace::point(int x, int y)
{
    return this->array->at(x)->at(y);
}

string VisualSpace::toString()
{
    return toString("double");
}

string VisualSpace::toString(string display_type)
{
    string result = ">";
    for(int y=this->getHieght()-1; y>= 0; y--)
    {
        for(int x=0; x<this->getWidth(); x++)
        {
            double value = this->point(x,y)->getValue();
            result += ":";
            if(value != 0)
            {
                string number;
                if(display_type == "double")
                {
                    number = to_string(double(value));
                }
                if(display_type == "int")
                {
                    number = to_string(int(value));
                }
                if(display_type == "float")
                {
                    number = to_string(float(value));
                }
                if(display_type == "uint")
                {
                    number = to_string(uint32_t(value));
                }
                if(display_type == "mark")
                {
                    number = "#";
                }
                if(number.find('.') != string::npos)
                {
                    while(number.back() == '0')
                    {
                        number.pop_back();
                    }
                    if(number.back() == '.')
                    {
                        number.pop_back();
                    }
                }
                result += number;
            }else{
                result += "_";
            }
        }
        result += ":";
        result += "<\n>";
    }
    result.pop_back();
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