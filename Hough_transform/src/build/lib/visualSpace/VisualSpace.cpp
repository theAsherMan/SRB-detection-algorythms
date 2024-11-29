#include "VisualSpace.hpp"

using namespace std;

VisualSpace::VisualSpace(int width, int height)
{
    this->array = valarray<valarray<VSPoint*>>(width);
    for(int ii=0; ii<width; ii++)
    {
        this->array[ii] = valarray<VSPoint*>(height);
        for(int jj=0; jj<height; jj++)
        {
            this->array[ii][jj] = new VSPoint(ii, jj, 0, this);
        }
    }

    this->width = width;
    this->hieght = height;

    setAsDirty();
}
VisualSpace::VisualSpace(const valarray<valarray<double>>* source)
{
    valarray<valarray<double>> de_ref_source = *source;
    this->width = de_ref_source.size();
    this->hieght = de_ref_source[0].size();
    this->array = valarray<valarray<VSPoint*>>(this->width);
    for(int ii=0; ii<width; ii++)
    {
        this->array[ii] = valarray<VSPoint*>(this->hieght);
    }

    setAsDirty();
}
VisualSpace::VisualSpace(int width, int height, const valarray<double>* source)
{
    auto de_ref_source = *source;
    this->width = width;
    this->hieght = height;

    this->array = valarray<valarray<VSPoint*>>(width);
    for(int ii=0; ii<width; ii++)
    {
        this->array[ii] = valarray<VSPoint*>(height);
        for(int jj=0; jj<height; jj++)
        {
            double value = de_ref_source[ii + width * jj];
            this->array[ii][jj] = new VSPoint(ii, jj, value, this);
        }
    }

    setAsDirty();
}
VisualSpace::~VisualSpace()
{
    for(int ii=0; ii<this->width; ii++)
    {
        for(int jj=0; jj<this->hieght; jj++)
        {
            delete(this->array[ii][jj]);
        }
    }
}

int VisualSpace::getWidth()
{
    return this->width;
}

int VisualSpace::getHieght()
{
    return this->hieght;
}

int VisualSpace::count()
{
    return width * hieght;
}

double VisualSpace::getImageRadius()
{
    double heightFromOrigin = double(hieght)/2;
    double widthFromOrigin = double(width)/2;

    return sqrt(pow(heightFromOrigin,2.0) + pow(widthFromOrigin,2.0));
}

VSPoint* VisualSpace::point(int x, int y)
{
    return this->array[x][y];
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

void VisualSpace::zeroAllNegitiveValues()
{
    setAsDirty();
    for(int ii=0; ii<this->getWidth(); ii++)
    {
        for(int jj=0; jj<this->getHieght(); jj++)
        {
            point(ii, jj)->setValueUnsafe(std::max(0.0, point(ii, jj)->getValue()));
        }
    }
}

void VisualSpace::standardize()
{
    double mean = this->mean();
    double std_dev = this->std_dev();

    for(int ii=0; ii<this->getWidth(); ii++)
    {
        for(int jj=0; jj<this->getHieght(); jj++)
        {
            this->point(ii, jj)->increaseValueUnsafe(-mean);
            this->point(ii, jj)->multiplyValueUnsafe(1/std_dev);
        }
    }
    setAsDirty();
}

void VisualSpace::standardizeIgnoringNonPositives()
{
    double mean = this->meanOfPositives();
    double std_dev = this->std_devOfPositives();

    for(int ii=0; ii<this->getWidth(); ii++)
    {
        for(int jj=0; jj<this->getHieght(); jj++)
        {
            this->point(ii, jj)->increaseValueUnsafe(-mean);
            this->point(ii, jj)->multiplyValueUnsafe(1/std_dev);
        }
    }
}

double VisualSpace::max()
{
    if(!dirty_max)
    {
        return _max;
    }
    _max = -INFINITY;
    for(int ii=0; ii<width; ii++)
    {
        for(int jj=0; jj<hieght; jj++)
        {
            _max = std::max(_max, array[ii][jj]->getValue());
        }
    }
    dirty_max = false;
    return _max;
}

double VisualSpace::min()
{
    if(!dirty_min)
    {
        return _min;
    }
    _min = INFINITY;
    for(int ii=0; ii<width; ii++)
    {
        for(int jj=0; jj<hieght; jj++)
        {
            _min = std::min(_min, array[ii][jj]->getValue());
        }
    }
    dirty_min = false;
    return _min;
}

double VisualSpace::mean()
{
    if(!dirty_mean)
    {
        return _mean;
    }
    _mean = 0;
    for(int ii=0; ii<width; ii++)
    {
        for(int jj=0; jj<hieght; jj++)
        {
            _mean += point(ii , jj)->getValue();
        }
    }
    _mean /= count();
    dirty_mean = false;
    return _mean;
}

double VisualSpace::meanOfPositives()
{
    if(!dirty_pos_mean)
    {
        return _meanOfPositives;
    }
    
    _meanOfPositives = 0;
    int _count = 0;
    for(int ii=0; ii<width; ii++)
    {
        for(int jj=0; jj<hieght; jj++)
        {
            double value = point(ii , jj)->getValue();
            if(value > 0)
            {
                _count++;
                _meanOfPositives += value;
            }
        }
    }
    
    _meanOfPositives /= _count;
    dirty_pos_mean = false;
    return _meanOfPositives;
}

double VisualSpace::std_dev()
{
    if(!dirty_dev)
    {
        return _std_dev;
    }
    double avg = mean();
    _std_dev = 0;
    for(int ii=0; ii<width; ii++)
    {
        for(int jj=0; jj<hieght; jj++)
        {
            _std_dev += pow(array[ii][jj]->getValue() - avg, 2);
        }
    }
    dirty_dev = false;
    _std_dev = sqrt(_std_dev / count());
    return _std_dev;
}

double VisualSpace::std_devOfPositives()
{
    if(!dirty_pos_dev)
    {
        return _std_devOfPositives;
    }
    double avg = meanOfPositives();
    int _count = 0;
    _std_devOfPositives = 0;
    for(int ii=0; ii<width; ii++)
    {
        for(int jj=0; jj<hieght; jj++)
        {
            double value = point(ii , jj)->getValue();
            if(value > 0)
            {
                _count++;
                _std_devOfPositives += pow(value - avg, 2);
            }
        }
    }
    _std_devOfPositives = sqrt(_std_devOfPositives / _count);
    dirty_pos_dev = false;
    return _std_devOfPositives;
}

void VisualSpace::setAsDirty()
{
    dirty_max = true;
    dirty_min = true;
    dirty_mean = true;
    dirty_dev = true;
    dirty_pos_dev = true;
    dirty_pos_mean = true;
}

void VisualSpace::clear()
{
    for(int ii=0; ii<width; ii++)
    {
        for(int jj=0; jj<hieght; jj++)
        {
            this->array[ii][jj]->setValue(0);
        }
    }
    
    setAsDirty();
}

// -------------------------------------------------------------------

VSPoint::VSPoint(int x, int y, double value, VisualSpace* space)
{
    this->space = space;
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
void VSPoint::increaseValue(double value)
{
    space->setAsDirty();
    increaseValueUnsafe(value);
}

void VSPoint::increaseValueUnsafe(double value)
{
    this->value += value;
}

void VSPoint::setValue(double value)
{
    space->setAsDirty();
    setValueUnsafe(value);
}

void VSPoint::setValueUnsafe(double value)
{
    this->value = value;
}

void VSPoint::multiplyValue(double value)
{
    space->setAsDirty();
    multiplyValueUnsafe(value);
}

void VSPoint::multiplyValueUnsafe(double value)
{
    this->value *= value;
}